import fs from "fs";
import path from "path";
import React from "react";
import {
    computed,
    observable,
    action,
    makeObservable,
    runInAction
    //,autorun
} from "mobx";
import { observer } from "mobx-react";
import { dialog, getCurrentWindow } from "@electron/remote";

import * as notification from "eez-studio-ui/notification";
import { Button } from "eez-studio-ui/button";

import {
    ClassInfo,
    IEezObject,
    EezObject,
    registerClass,
    PropertyType,
    MessageType,
    PropertyProps,
    IMessage
} from "project-editor/core/object";
import { validators } from "eez-studio-shared/validation";

import {
    createObject,
    getProjectStore,
    getUniquePropertyValue,
    Message,
    ProjectStore,
    propertyNotSetMessage
} from "project-editor/store";

import { getThemedColor } from "project-editor/features/style/theme";

import { showGenericDialog } from "project-editor/core/util";

import { AbsoluteFileInput } from "project-editor/ui-components/FileInput";
import { getProject, findStyle } from "project-editor/project/project";

import { ProjectEditor } from "project-editor/project-editor-interface";
import { generalGroup } from "project-editor/ui-components/PropertyGrid/groups";
import {
    BitmapColorFormat,
    isDashboardProject,
    isLVGLProject
} from "project-editor/project/project-type-traits";
import { IFieldProperties } from "eez-studio-types";
import type { ProjectEditorFeature } from "project-editor/store/features";

////////////////////////////////////////////////////////////////////////////////

const ExportBitmapFilePropertyGridUI = observer(
    class ExportBitmapFilePropertyGridUI extends React.Component<PropertyProps> {
        export = async () => {
            const bitmap = this.props.objects[0] as Bitmap;

            // for example: data:image/png;base64,
            const i = bitmap.image.indexOf("/");
            const j = bitmap.image.indexOf(";");
            const k = bitmap.image.indexOf(",");

            const ext = bitmap.image.substring(i + 1, j);
            console.log(ext);
            console.log(bitmap.name);

            const result = await dialog.showSaveDialog(getCurrentWindow(), {
                filters: [{ name: "All Files", extensions: ["*"] }],
                defaultPath: bitmap.name + "." + ext
            });
            let filePath = result.filePath;

            if (filePath) {
                const bin = Buffer.from(
                    bitmap.image.substring(k + 1),
                    "base64"
                );
                try {
                    await fs.promises.writeFile(filePath, bin);
                    notification.info(`Bitmap file exported.`);
                } catch (error) {
                    notification.error(error.toString());
                }
            }
        };

        render() {
            if (this.props.objects.length > 1) {
                return null;
            }
            return (
                <div style={{ marginTop: 10 }}>
                    <Button color="primary" size="small" onClick={this.export}>
                        Export Bitmap File
                    </Button>
                </div>
            );
        }
    }
);

////////////////////////////////////////////////////////////////////////////////

export const CF_ALPHA_1_BIT = 1;
export const CF_ALPHA_2_BIT = 2;
export const CF_ALPHA_4_BIT = 3;
export const CF_ALPHA_8_BIT = 4;

export const CF_INDEXED_1_BIT = 41;
export const CF_INDEXED_2_BIT = 42;
export const CF_INDEXED_4_BIT = 43;
export const CF_INDEXED_8_BIT = 44;

export const CF_RAW = 51;
export const CF_RAW_CHROMA = 52;
export const CF_RAW_ALPHA = 53;

export const CF_TRUE_COLOR = 24;
export const CF_TRUE_COLOR_ALPHA = 32;
export const CF_TRUE_COLOR_CHROMA = 33;

export const CF_RGB565A8 = 16;

const LVGL_COLOR_FORMATS = [
    { id: CF_ALPHA_1_BIT, label: "ALPHA 1 BIT" },
    { id: CF_ALPHA_2_BIT, label: "ALPHA 2 BIT" },
    { id: CF_ALPHA_4_BIT, label: "ALPHA 4 BIT" },
    { id: CF_ALPHA_8_BIT, label: "ALPHA 8 BIT" },

    { id: CF_INDEXED_1_BIT, label: "INDEXED 1 BIT" },
    { id: CF_INDEXED_2_BIT, label: "INDEXED 2 BIT" },
    { id: CF_INDEXED_4_BIT, label: "INDEXED 4 BIT" },
    { id: CF_INDEXED_8_BIT, label: "INDEXED 8 BIT" },

    { id: CF_RAW, label: "RAW" },
    { id: CF_RAW_CHROMA, label: "RAW CHROMA" },
    { id: CF_RAW_ALPHA, label: "RAW ALPHA" },

    { id: CF_TRUE_COLOR, label: "TRUE COLOR" },
    {
        id: CF_TRUE_COLOR_ALPHA,
        label: "TRUE COLOR ALPHA"
    },
    {
        id: CF_TRUE_COLOR_CHROMA,
        label: "TRUE COLOR CHROMA"
    },

    { id: CF_RGB565A8, label: "RGB565A8" }
];

export class Bitmap extends EezObject {
    id: number | undefined;
    name: string;
    description?: string;
    image: string;
    bpp: number;
    alwaysBuild: boolean;
    style?: string;

    constructor() {
        super();

        makeObservable<Bitmap, "_imageElement">(this, {
            id: observable,
            name: observable,
            description: observable,
            image: observable,
            bpp: observable,
            alwaysBuild: observable,
            style: observable,
            _imageElement: observable,
            backgroundColor: computed,
            imageElement: computed({ keepAlive: true }),
            bitmapData: computed({ keepAlive: true })
        });
    }

    static classInfo: ClassInfo = {
        properties: [
            {
                name: "id",
                type: PropertyType.Number,
                isOptional: true,
                unique: true,
                propertyGridGroup: generalGroup,
                hideInPropertyGrid: (bitmap: Bitmap) =>
                    isLVGLProject(bitmap) || isDashboardProject(bitmap)
            },
            {
                name: "name",
                type: PropertyType.String,
                unique: true
            },
            {
                name: "description",
                type: PropertyType.MultilineText
            },
            {
                name: "image",
                type: PropertyType.Image,
                skipSearch: true,
                disableBitmapPreview: true,
                embeddedImage: true
            },
            {
                name: "bpp",
                displayName: (bitmap: Bitmap) =>
                    isLVGLProject(bitmap) ? "Color format" : "Bits per pixel",
                type: PropertyType.Enum,
                enumItems: (bitmap: Bitmap) =>
                    isLVGLProject(bitmap)
                        ? LVGL_COLOR_FORMATS
                        : [{ id: 16 }, { id: 32 }],
                defaultValue: 16,
                hideInPropertyGrid: isDashboardProject
            },
            {
                name: "style",
                type: PropertyType.ObjectReference,
                referencedObjectCollectionPath: "allStyles",
                hideInPropertyGrid: (bitmap: Bitmap) =>
                    isLVGLProject(bitmap) ||
                    isDashboardProject(bitmap) ||
                    bitmap.bpp == 32
            },
            {
                name: "alwaysBuild",
                displayName: "Always add to the generated code",
                type: PropertyType.Boolean,
                hideInPropertyGrid: object =>
                    isLVGLProject(object) || isDashboardProject(object)
            },
            {
                name: "customUI",
                type: PropertyType.Any,
                computed: true,
                propertyGridRowComponent: ExportBitmapFilePropertyGridUI,
                hideInPropertyGrid: (bitmap: Bitmap) =>
                    bitmap.image &&
                    typeof bitmap.image == "string" &&
                    bitmap.image.startsWith("data:image/")
                        ? false
                        : true
            }
        ],
        propertiesPanelLabel: (bitmap: Bitmap) => {
            return `Bitmap: ${bitmap.name}`;
        },
        check: (bitmap: Bitmap, messages: IMessage[]) => {
            const projectStore = getProjectStore(bitmap);

            if (!bitmap.image) {
                messages.push(propertyNotSetMessage(bitmap, "image"));
            }

            ProjectEditor.checkAssetId(
                projectStore,
                "bitmaps",
                bitmap,
                messages
            );
        },
        newItem: async (parent: IEezObject) => {
            const projectStore = getProjectStore(parent);

            const result = await showGenericDialog(projectStore, {
                dialogDefinition: {
                    title: "New Bitmap",
                    fields: [
                        {
                            name: "name",
                            type: "string",
                            validators: [
                                validators.required,
                                validators.invalidCharacters("."),
                                validators.unique({}, parent)
                            ]
                        },
                        {
                            name: "imageFilePath",
                            displayName: "Image",
                            type: AbsoluteFileInput,
                            validators: [validators.required],
                            options: {
                                filters: [
                                    {
                                        name: "Image files",
                                        extensions: ["png", "jpg", "jpeg"]
                                    },
                                    { name: "All Files", extensions: ["*"] }
                                ]
                            }
                        },
                        ...(projectStore.projectTypeTraits.isLVGL
                            ? [
                                  {
                                      name: "bpp",
                                      displayName: "Color format",
                                      type: "enum",
                                      enumItems: LVGL_COLOR_FORMATS
                                  } as IFieldProperties
                              ]
                            : projectStore.projectTypeTraits.isDashboard
                            ? []
                            : [
                                  {
                                      name: "bpp",
                                      displayName: "Bits per pixel",
                                      type: "enum",
                                      enumItems: [16, 32]
                                  } as IFieldProperties
                              ])
                    ]
                },
                values: {
                    bpp: projectStore.projectTypeTraits.isLVGL
                        ? CF_TRUE_COLOR_ALPHA
                        : 32
                }
            });

            const name: string = result.values.name;
            const bpp: number = result.values.bpp;

            return createBitmap(
                projectStore,
                result.values.imageFilePath,
                undefined,
                name,
                bpp
            );
        },
        icon: "material:image",
        afterLoadHook: (bitmap: Bitmap, project) => {
            bitmap.migrateLvglBitmap(project._store);
        },
        updateObjectValueHook: (bitmap: Bitmap, values: any) => {
            if (values.image != undefined && bitmap.image != values.image) {
                const project = ProjectEditor.getProject(bitmap);

                if (project.projectTypeTraits.isLVGL) {
                    project._store.updateObject(bitmap, {});
                }
            }
        }

        /*,
        // MIGRATION TO LOW RES
        beforeLoadHook: (bitmap: Bitmap, jsWidget: Partial<Bitmap>) => {
            const dispose = autorun(() => {
                const image = bitmap.imageElement;
                if (image) {
                    dispose();

                    setTimeout(() => {
                        let canvas = document.createElement("canvas");
                        canvas.width = Math.floor((image.width * 480) / 800);
                        canvas.height = Math.floor((image.height * 480) / 800);

                        let ctx = canvas.getContext("2d");
                        if (ctx == null) {
                            return;
                        }

                        if (bitmap.backgroundColor !== "transparent") {
                            ctx.fillStyle = bitmap.backgroundColor;
                            ctx.fillRect(0, 0, canvas.width, canvas.height);
                        } else {
                            ctx.clearRect(0, 0, canvas.width, canvas.height);
                        }

                        ctx.drawImage(image, 0, 0, canvas.width, canvas.height);

                        runInAction(() => {
                            bitmap.image = canvas.toDataURL();
                        });
                    }, 1000);
                }
            });
        }
        */
    };

    private _imageElement: HTMLImageElement | null | undefined = undefined;
    private _imageElementImage: string;

    get backgroundColor() {
        if (
            !isLVGLProject(this) &&
            !isDashboardProject(this) &&
            this.bpp !== 32
        ) {
            const style = findStyle(
                ProjectEditor.getProject(this),
                this.style || "default"
            );
            if (style && style.backgroundColorProperty) {
                return getThemedColor(
                    getProjectStore(this),
                    style.backgroundColorProperty
                );
            }
        }
        return "transparent";
    }

    get imageSrc() {
        if (!this.image) {
            return "";
        }

        if (this.image.startsWith("data:image/")) {
            return this.image;
        }

        return ProjectEditor.getProjectStore(this).getAbsoluteFilePath(
            this.image
        );
    }

    get imageElement() {
        if (!this.image) {
            return null;
        }

        if (
            this._imageElement === undefined ||
            this.image !== this._imageElementImage
        ) {
            let imageElement = new Image();
            imageElement.src = this.imageSrc;

            imageElement.onload = action(() => {
                this._imageElement = imageElement;
                this._imageElementImage = this.image;
            });

            imageElement.onerror = action(() => {
                this._imageElement = null;
                this._imageElementImage = this.image;
            });

            return undefined;
        }

        return this._imageElement;
    }

    getBitmapData(bpp: number) {
        const image = this.imageElement;
        if (!(image instanceof HTMLImageElement)) {
            return image;
        }

        let canvas = document.createElement("canvas");
        canvas.width = image.width;
        canvas.height = image.height;

        let ctx = canvas.getContext("2d");
        if (ctx == null) {
            return undefined;
        }

        if (this.backgroundColor !== "transparent") {
            ctx.fillStyle = this.backgroundColor;
            ctx.fillRect(0, 0, image.width, image.height);
        } else {
            ctx.clearRect(0, 0, image.width, image.height);
        }

        ctx.drawImage(image, 0, 0);

        let imageData = ctx.getImageData(0, 0, image.width, image.height).data;

        const isLVGL = isLVGLProject(this);

        const bytesPerPixel = bpp == 32 ? 4 : bpp == 24 ? 3 : isLVGL ? 3 : 2; // for LVGL 16 bit is actually RGB565A8 (24 bit)

        let pixels = new Uint8Array(bytesPerPixel * image.width * image.height);

        const rgb =
            getProject(this).projectTypeTraits.bitmapColorFormat ==
            BitmapColorFormat.RGB;

        for (let i = 0; i < 4 * image.width * image.height; i += 4) {
            let r = imageData[i];
            let g = imageData[i + 1];
            let b = imageData[i + 2];
            let a = imageData[i + 3];

            if (bpp === 32) {
                if (rgb) {
                    pixels[i] = r;
                    pixels[i + 1] = g;
                    pixels[i + 2] = b;
                    pixels[i + 3] = a;
                } else {
                    pixels[i] = b;
                    pixels[i + 1] = g;
                    pixels[i + 2] = r;
                    pixels[i + 3] = a;
                }
            } else if (bpp == 24) {
                if (rgb) {
                    pixels[3 * (i / 4) + 0] = r;
                    pixels[3 * (i / 4) + 1] = g;
                    pixels[3 * (i / 4) + 2] = b;
                } else {
                    pixels[3 * (i / 4) + 0] = b;
                    pixels[3 * (i / 4) + 1] = g;
                    pixels[3 * (i / 4) + 2] = r;
                }
            } else {
                // rrrrrggggggbbbbb
                pixels[i / 2] = ((g & 28) << 3) | (b >> 3);
                pixels[i / 2 + 1] = (r & 248) | (g >> 5);

                if (isLVGL) {
                    pixels[2 * image.width * image.height + i / 4] = a;
                }
            }
        }

        return {
            width: image.width,
            height: image.height,
            bpp,
            pixels
        };
    }

    get bitmapData() {
        return this.getBitmapData(this.bpp);
    }

    async migrateLvglBitmap(projectStore: ProjectStore) {
        if (this.image.startsWith("data:image/")) {
            return;
        }

        // migrate from assets folder to the embedded asset

        const absoluteFilePath = projectStore.getAbsoluteFilePath(this.image);

        const imageData = await fs.promises.readFile(
            absoluteFilePath,
            "base64"
        );

        const ext = path.extname(absoluteFilePath).toLowerCase();
        let fileType: string;
        if (ext == ".jpg" || ext == ".jpeg") {
            fileType = "image/jpg";
        } else {
            fileType = "image/png";
        }

        runInAction(() => {
            this.image = `data:${fileType};base64,` + imageData;
            projectStore.setModified(Symbol());
        });
    }
}

registerClass("Bitmap", Bitmap);

export async function createBitmap(
    projectStore: ProjectStore,
    filePath: string,
    fileType?: string,
    name?: string,
    bpp?: number
) {
    if (fileType == undefined) {
        const ext = path.extname(filePath).toLowerCase();
        if (ext == ".jpg" || ext == ".jpeg") {
            fileType = "image/jpg";
        } else {
            fileType = "image/png";
        }
    }

    if (bpp == undefined) {
        if (fileType == "image/jpg") {
            bpp = 32; // 24
        } else {
            bpp = 32;
        }
    }

    if (!name) {
        name = getUniquePropertyValue(
            projectStore.project.bitmaps,
            "name",
            path.parse(filePath).name
        ) as string;
    }

    try {
        const result = fs.readFileSync(filePath, "base64");

        const bitmapProperties: Partial<Bitmap> = {
            name,
            image: `data:${fileType};base64,` + result,
            bpp,
            alwaysBuild: false
        };

        const bitmap = createObject<Bitmap>(
            projectStore,
            bitmapProperties,
            Bitmap
        );

        return bitmap;
    } catch (err) {
        notification.error(err);
        return undefined;
    }
}

////////////////////////////////////////////////////////////////////////////////

export interface BitmapData {
    width: number;
    height: number;
    bpp: number;
    style?: string;
    pixels: Uint8Array;
}

export function getBitmapData(
    bitmap: Bitmap,
    bppOverride?: number
): BitmapData {
    const bitmapData =
        bppOverride != undefined
            ? bitmap.getBitmapData(bppOverride)
            : bitmap.bitmapData;

    if (bitmapData) {
        return bitmapData;
    }

    return {
        width: 1,
        height: 1,
        bpp: 32,
        pixels: new Uint8Array([0, 0, 0, 0])
    };
}

// this function makes sure that bitmap is eventually loaded
export async function getBitmapDataAsync(
    bitmap: Bitmap,
    bppOverride?: number
): Promise<BitmapData> {
    while (true) {
        const bitmapData =
            bppOverride != undefined
                ? bitmap.getBitmapData(bppOverride)
                : bitmap.bitmapData;
        if (bitmapData) {
            return bitmapData;
        }
        if (bitmapData === null) {
            return {
                width: 1,
                height: 1,
                bpp: 32,
                pixels: new Uint8Array([0, 0, 0, 0])
            };
        }

        // still loading, wait for 10 ms
        await new Promise(resolve => setTimeout(resolve, 10));
    }
}

export async function preloadAllBitmaps(projectStore: ProjectStore) {
    const bitmaps: Bitmap[] = [];
    for (const project of projectStore.project._store.openProjectsManager
        .projects) {
        bitmaps.push(...project.bitmaps);
    }

    return Promise.all(bitmaps.map(bitmap => getBitmapDataAsync(bitmap)));
}

////////////////////////////////////////////////////////////////////////////////

const feature: ProjectEditorFeature = {
    name: "eezstudio-project-feature-bitmap",
    version: "0.1.0",
    description: "Bitmaps support for your project",
    author: "EEZ",
    authorLogo: "../eez-studio-ui/_images/eez_logo.png",
    displayName: "Bitmaps",
    mandatory: false,
    key: "bitmaps",
    type: PropertyType.Array,
    typeClass: Bitmap,
    icon: "material:image",
    create: () => [],
    check: (projectStore, object: EezObject[], messages: IMessage[]) => {
        if (object.length > 65535) {
            messages.push(
                new Message(
                    MessageType.ERROR,
                    "Max. 65535 bitmaps are supported",
                    object
                )
            );
        }

        if (
            !projectStore.projectTypeTraits.isDashboard &&
            !projectStore.projectTypeTraits.isLVGL &&
            !findStyle(projectStore.project, "default")
        ) {
            messages.push(
                new Message(
                    MessageType.ERROR,
                    "'Default' style is missing.",
                    object
                )
            );
        }
    }
};

export default feature;
