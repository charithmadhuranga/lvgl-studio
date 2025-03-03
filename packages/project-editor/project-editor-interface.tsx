import type { ProjectEditorTab, Tabs } from "home/tabs-store";
import type { ProjectEditorFeature } from "project-editor/store/features";
import type { IDocumentSearch } from "project-editor/core/search";
import type { DataContext } from "project-editor/features/variable/variable";
import type { RemoteRuntime } from "project-editor/flow/runtime/remote-runtime";
import type { WasmRuntime } from "project-editor/flow/runtime/wasm-runtime";
import type { DebugInfoRuntime } from "project-editor/flow/runtime/debug-info-runtime";
import type {
    build,
    backgroundCheck,
    buildExtensions
} from "project-editor/build/build";
import type {
    checkAssetId,
    getFlow,
    getProject,
    Project,
    ImportDirective
} from "project-editor/project/project";
import type { Page } from "project-editor/features/page/page";
import type {
    ActionComponent,
    Component,
    Widget,
    getWidgetParent,
    CustomInput,
    CustomOutput,
    createActionComponentClass
} from "project-editor/flow/component";
import type {
    Glyph,
    rebuildLvglFonts
} from "project-editor/features/font/font";
import type { Flow, FlowFragment } from "project-editor/flow/flow";
import type { ConnectionLine } from "project-editor/flow/connection-line";
import type { Action } from "project-editor/features/action/action";
import type {
    ScpiCommand,
    ScpiSubsystem
} from "project-editor/features/scpi/scpi";
import type { getObjectVariableTypeFromType } from "project-editor/features/variable/value-type";
import type { getBitmapData } from "project-editor/features/bitmap/bitmap";
import type {
    migrateProjectVersion,
    migrateProjectType
} from "project-editor/project/migrate-project";
import type {
    getNavigationObject,
    navigateTo,
    selectObject
} from "project-editor/project/ui/NavigationComponentFactory";
import type {
    createEditorState,
    getEditorComponent,
    getAncestorWithEditorComponent
} from "project-editor/project/ui/EditorComponentFactory";
import type { browseGlyph } from "project-editor/features/font/FontEditor";
import type { Variable } from "project-editor/features/variable/variable";
import type {
    OutputActionComponent,
    CallActionActionComponent
} from "project-editor/flow/components/actions";
import type {
    ContainerWidget,
    UserWidgetWidget,
    ListWidget,
    SelectWidget
} from "project-editor/flow/components/widgets";
import type { ArrayProperty } from "project-editor/ui-components/PropertyGrid/ArrayElementProperty";
import type { EmbeddedPropertyGrid } from "project-editor/ui-components/PropertyGrid/EmbeddedPropertyGrid";
import type {
    LVGLWidget,
    LVGLPanelWidget,
    LVGLUserWidgetWidget
} from "project-editor/lvgl/widgets";
import type { LVGLStyle } from "project-editor/lvgl/style";
import type { Property } from "project-editor/ui-components/PropertyGrid/Property";
import type { getProjectStore } from "project-editor/store";

import type {
    Structure,
    StructureField,
    Enum,
    EnumMember
} from "project-editor/features/variable/variable";

import type { Style } from "project-editor/features/style/style";

export interface IProjectEditor {
    homeTabs?: Tabs;
    ProjectEditorTabClass: typeof ProjectEditorTab;
    DataContextClass: typeof DataContext;
    extensions: ProjectEditorFeature[];
    documentSearch: IDocumentSearch;
    RemoteRuntimeClass: typeof RemoteRuntime;
    WasmRuntimeClass: typeof WasmRuntime;
    DebugInfoRuntimeClass: typeof DebugInfoRuntime;
    build: {
        buildProject: typeof build;
        backgroundCheck: typeof backgroundCheck;
        buildExtensions: typeof buildExtensions;
    };
    ProjectClass: typeof Project;
    FlowClass: typeof Flow;
    FlowFragmentClass: typeof FlowFragment;
    PageClass: typeof Page;
    ActionClass: typeof Action;
    ComponentClass: typeof Component;
    ActionComponentClass: typeof ActionComponent;
    WidgetClass: typeof Widget;
    ConnectionLineClass: typeof ConnectionLine;
    UserWidgetWidgetClass: typeof UserWidgetWidget;
    SelectWidgetClass: typeof SelectWidget;
    ContainerWidgetClass: typeof ContainerWidget;
    ListWidgetClass: typeof ListWidget;
    OutputActionComponentClass: typeof OutputActionComponent;
    CallActionActionComponentClass: typeof CallActionActionComponent;
    VariableClass: typeof Variable;
    GlyphClass: typeof Glyph;
    ScpiCommandClass: typeof ScpiCommand;
    ScpiSubsystemClass: typeof ScpiSubsystem;
    StyleClass: typeof Style;
    LVGLWidgetClass: typeof LVGLWidget;
    LVGLPanelWidgetClass: typeof LVGLPanelWidget;
    LVGLUserWidgetWidgetClass: typeof LVGLUserWidgetWidget;
    LVGLStyleClass: typeof LVGLStyle;
    getProject: typeof getProject;
    getProjectStore: typeof getProjectStore;
    getFlow: typeof getFlow;
    getObjectVariableTypeFromType: typeof getObjectVariableTypeFromType;
    getWidgetParent: typeof getWidgetParent;
    rebuildLvglFonts: typeof rebuildLvglFonts;
    getBitmapData: typeof getBitmapData;
    migrateProjectVersion: typeof migrateProjectVersion;
    migrateProjectType: typeof migrateProjectType;
    getNavigationObject: typeof getNavigationObject;
    navigateTo: typeof navigateTo;
    selectObject: typeof selectObject;
    getEditorComponent: typeof getEditorComponent;
    getAncestorWithEditorComponent: typeof getAncestorWithEditorComponent;
    createEditorState: typeof createEditorState;
    browseGlyph: typeof browseGlyph;
    checkAssetId: typeof checkAssetId;
    Property: typeof Property;
    ArrayProperty: typeof ArrayProperty;
    EmbeddedPropertyGrid: typeof EmbeddedPropertyGrid;
    StructureClass: typeof Structure;
    StructureFieldClass: typeof StructureField;
    EnumClass: typeof Enum;
    EnumMemberClass: typeof EnumMember;
    CustomInputClass: typeof CustomInput;
    CustomOutputClass: typeof CustomOutput;
    ImportDirectiveClass: typeof ImportDirective;
    createActionComponentClass: typeof createActionComponentClass;
}

export const ProjectEditor: IProjectEditor = {} as any;
