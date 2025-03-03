import { observable, runInAction, autorun, makeObservable } from "mobx";

import {
    isDev,
    getUserDataPath,
    fileExists,
    readJsObjectFromFile,
    writeJsObjectToFile
} from "eez-studio-shared/util-electron";

import * as notification from "eez-studio-ui/notification";

import { IExtension } from "eez-studio-shared/extensions/extension";

import { sourceRootDir } from "eez-studio-shared/util";
import { firstTime } from "home/first-time";

export const DEFAULT_EXTENSIONS_CATALOG_VERSION_DOWNLOAD_URL =
    "https://github.com/eez-open/studio-extensions/raw/master/build/catalog-version.json";

export const DEFAULT_EXTENSIONS_CATALOG_DOWNLOAD_URL =
    "https://github.com/eez-open/studio-extensions/raw/master/build/catalog.zip";

interface ICatalogVersion {
    lastModified: Date;
}

class ExtensionsCatalog {
    catalog: IExtension[] = [];
    catalogVersion: ICatalogVersion;

    constructor() {
        makeObservable(this, {
            catalog: observable
        });
    }

    load() {
        this._loadCatalog()
            .then(catalog => {
                runInAction(() => (this.catalog = catalog));
            })
            .catch(error =>
                notification.error(
                    `Failed to load extensions catalog (${error})`
                )
            );

        this._loadCatalogVersion()
            .then(catalogVersion => {
                runInAction(() => (this.catalogVersion = catalogVersion));

                if (firstTime.get()) {
                    const dispose = autorun(() => {
                        if (!firstTime.get()) {
                            dispose();
                            this.checkNewVersionOfCatalog();
                        }
                    });
                } else {
                    this.checkNewVersionOfCatalog();
                }
            })
            .catch(error =>
                notification.error(`Failed to load catalog version (${error})`)
            );
    }

    get catalogPath() {
        return getUserDataPath("catalog.json");
    }

    async _loadCatalog() {
        let catalogPath = this.catalogPath;
        if (!(await fileExists(catalogPath))) {
            if (isDev) {
                catalogPath = `${sourceRootDir()}/../resources/catalog.json`;
            } else {
                catalogPath = process.resourcesPath! + "/catalog.json";
            }
        }
        return (await readJsObjectFromFile(catalogPath)) as IExtension[];
    }

    get catalogVersionPath() {
        return getUserDataPath("catalog-version.json");
    }

    async _loadCatalogVersion() {
        let catalogVersion;

        let catalogVersionPath = this.catalogVersionPath;
        if (await fileExists(catalogVersionPath)) {
            try {
                catalogVersion = await readJsObjectFromFile(catalogVersionPath);
            } catch (err) {
                console.error(err);
            }
        }

        if (!catalogVersion) {
            if (isDev) {
                catalogVersionPath = `${sourceRootDir()}/../resources/catalog-version.json`;
            } else {
                catalogVersionPath =
                    process.resourcesPath! + "/catalog-version.json";
            }
        }

        catalogVersion = await readJsObjectFromFile(catalogVersionPath);

        catalogVersion.lastModified = new Date(catalogVersion.lastModified);

        return catalogVersion;
    }

    async checkNewVersionOfCatalog() {
        try {
            const catalogVersion = await this.downloadCatalogVersion();

            if (
                !this.catalogVersion ||
                catalogVersion.lastModified > this.catalogVersion.lastModified
            ) {
                runInAction(() => (this.catalogVersion = catalogVersion));
                this.downloadCatalog();
            } else {
                // no new version
                return false;
            }
        } catch (error) {
            console.error(error);
            notification.error(`Failed to download extensions catalog version`);
        }

        return true;
    }

    downloadCatalogVersion() {
        return new Promise<ICatalogVersion>((resolve, reject) => {
            var req = new XMLHttpRequest();
            req.responseType = "json";
            req.open("GET", DEFAULT_EXTENSIONS_CATALOG_VERSION_DOWNLOAD_URL);

            req.addEventListener("load", async () => {
                const catalogVersion = req.response;
                catalogVersion.lastModified = new Date(
                    catalogVersion.lastModified
                );
                await writeJsObjectToFile(
                    this.catalogVersionPath,
                    catalogVersion
                );
                resolve(catalogVersion);
            });

            req.addEventListener("error", error => {
                console.error(
                    "Failed to download catalog-version.json for extensions",
                    error
                );
                reject(error);
            });

            req.send();
        });
    }

    downloadCatalog() {
        var req = new XMLHttpRequest();
        req.responseType = "arraybuffer";
        req.open("GET", DEFAULT_EXTENSIONS_CATALOG_DOWNLOAD_URL);

        const progressToastId = notification.info(
            "Downloading extensions catalog ...",
            {
                autoClose: false,
                hideProgressBar: false
            }
        );

        req.addEventListener("progress", event => {
            notification.update(progressToastId, {
                render: event.total
                    ? `Downloading extensions catalog: ${event.loaded} of ${event.total}`
                    : `Downloading extensions catalog: ${event.loaded}`
            });
        });

        req.addEventListener("load", async () => {
            const decompress = require("decompress");

            const files = await decompress(Buffer.from(req.response));

            const catalog = JSON.parse(files[0].data);

            runInAction(() => (this.catalog = catalog));

            await writeJsObjectToFile(this.catalogPath, this.catalog);

            notification.update(progressToastId, {
                type: notification.SUCCESS,
                render: `The latest extensions catalog successfully downloaded.`,
                autoClose: 5000
            });
        });

        req.addEventListener("error", error => {
            console.error("ExtensionsCatalog download error", error);
            notification.update(progressToastId, {
                type: notification.ERROR,
                render: `Failed to download extensions catalog.`,
                autoClose: 5000
            });
        });

        req.send();
    }
}

export const extensionsCatalog = new ExtensionsCatalog();
