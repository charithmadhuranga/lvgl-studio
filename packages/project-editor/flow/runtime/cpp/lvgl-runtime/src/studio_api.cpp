#include <stdlib.h>
#include <stdio.h>
#include <emscripten.h>

#include "lvgl/lvgl.h"

#include <eez/core/os.h>

#include "flow.h"

EM_PORT_API(lv_obj_t *) lvglCreateContainer(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_obj_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateLabel(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, char *text, lv_label_long_mode_t long_mode, bool recolor) {
    lv_obj_t *obj = lv_label_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    if (text != 0) {
        lv_label_set_text(obj, text);
        free(text);
    }
    lv_label_set_long_mode(obj, long_mode);
    lv_label_set_recolor(obj, recolor);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateButton(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_btn_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreatePanel(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_obj_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateUserWidget(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_obj_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);

    lv_style_value_t value;
    value.num = 0;
    lv_obj_set_local_style_prop(obj, LV_STYLE_PAD_LEFT, value, LV_PART_MAIN);
    lv_obj_set_local_style_prop(obj, LV_STYLE_PAD_TOP, value, LV_PART_MAIN);
    lv_obj_set_local_style_prop(obj, LV_STYLE_PAD_RIGHT, value, LV_PART_MAIN);
    lv_obj_set_local_style_prop(obj, LV_STYLE_PAD_LEFT, value, LV_PART_MAIN);
    lv_obj_set_local_style_prop(obj, LV_STYLE_BG_OPA, value, LV_PART_MAIN);
    lv_obj_set_local_style_prop(obj, LV_STYLE_BORDER_WIDTH, value, LV_PART_MAIN);

    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateImage(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const void *img_src, lv_coord_t pivotX, lv_coord_t pivotY, uint16_t zoom, int16_t angle) {
    lv_obj_t *obj = lv_img_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    if (img_src != 0) {
        lv_img_set_src(obj, img_src);
    }
    lv_img_set_pivot(obj, pivotX, pivotY);
    lv_img_set_zoom(obj, zoom);
    lv_img_set_angle(obj, angle);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateSlider(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, int32_t min, int32_t max, lv_slider_mode_t mode, int32_t value, int32_t value_left) {
    lv_obj_t *obj = lv_slider_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_slider_set_range(obj, min, max);
    lv_slider_set_mode(obj, mode);
    lv_slider_set_value(obj, value, LV_ANIM_OFF);
    if (lv_slider_get_mode(obj) == LV_SLIDER_MODE_RANGE) {
        lv_slider_set_left_value(obj, value_left, LV_ANIM_OFF);
    }
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateRoller(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, char *options, uint16_t selected, lv_roller_mode_t mode) {
    lv_obj_t *obj = lv_roller_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_roller_set_selected(obj, selected, LV_ANIM_OFF);
    lv_roller_set_options(obj, options, mode);
    free(options);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateSwitch(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_switch_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateBar(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, int32_t min, int32_t max, lv_bar_mode_t mode, int32_t value, int32_t value_left) {
    lv_obj_t *obj = lv_bar_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_bar_set_range(obj, min, max);
    lv_bar_set_mode(obj, mode);
    lv_bar_set_value(obj, value, LV_ANIM_OFF);
    if (lv_bar_get_mode(obj) == LV_BAR_MODE_RANGE) {
        lv_bar_set_start_value(obj, value_left, LV_ANIM_OFF);
    }
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateDropdown(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, char *options, uint16_t selected) {
    lv_obj_t *obj = lv_dropdown_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_dropdown_set_options(obj, options);
    lv_dropdown_set_selected(obj, selected);
    free(options);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateArc(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, int32_t range_min, int32_t range_max, int32_t value, int32_t bg_start_angle, int32_t bg_end_angle, lv_bar_mode_t mode, int32_t rotation) {
    lv_obj_t *obj = lv_arc_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_arc_set_range(obj, range_min, range_max);
    lv_arc_set_value(obj, value);
    lv_arc_set_bg_angles(obj, bg_start_angle, bg_end_angle);
    lv_arc_set_mode(obj, mode);
    lv_arc_set_rotation(obj, rotation);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateSpinner(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_spinner_create(parentObj, 1000, 60);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    if (is_editor) {
        lv_anim_del_all();
    }
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateCheckbox(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char *text) {
    lv_obj_t *obj = lv_checkbox_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    lv_checkbox_set_text(obj, text);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateTextarea(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, char *text, char *placeholder, bool one_line_mode, bool password_mode, char *accepted_characters, uint32_t max_text_length) {
    lv_obj_t *obj = lv_textarea_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    if (accepted_characters) {
        lv_textarea_set_accepted_chars(obj, accepted_characters);
    }
    lv_textarea_set_max_length(obj, max_text_length);
    if (text != 0) {
        lv_textarea_set_text(obj, text);
        free(text);
    }
    if (placeholder) {
        lv_textarea_set_placeholder_text(obj, placeholder);
        free(placeholder);
    }
    lv_textarea_set_one_line(obj, one_line_mode);
    lv_textarea_set_password_mode(obj, password_mode);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateCalendar(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, uint32_t today_year, uint32_t today_month, uint32_t today_day, uint32_t showed_year, uint32_t showed_month) {
    lv_obj_t *obj = lv_calendar_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    lv_calendar_header_arrow_create(obj);
    lv_calendar_set_today_date(obj, today_year, today_month, today_day);
    lv_calendar_set_showed_date(obj, showed_year, showed_month);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateColorwheel(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_colorwheel_mode_t mode, bool fixed_mode) {
    lv_obj_t *obj = lv_colorwheel_create(parentObj, false);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_colorwheel_set_mode(obj, mode);
    lv_colorwheel_set_mode_fixed(obj, fixed_mode);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateImgbutton(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_imgbtn_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateKeyboard(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_keyboard_mode_t mode) {
    lv_obj_t *obj = lv_keyboard_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_keyboard_set_mode(obj, mode);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateChart(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_chart_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

EM_PORT_API(lv_obj_t *) lvglCreateMeter(lv_obj_t *parentObj, int32_t index, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t *obj = lv_meter_create(parentObj);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_update_layout(obj);
    setObjectIndex(obj, index);
    return obj;
}

static lv_obj_t *current_screen = 0;

EM_PORT_API(void) lvglScreenLoad(unsigned page_index, lv_obj_t *obj) {
    lv_scr_load_anim(obj, (lv_scr_load_anim_t)screenLoad_animType, screenLoad_speed, screenLoad_delay, false);
    current_screen = obj;
    screenLoad_animType = 0;
    screenLoad_speed = 0;
    screenLoad_delay = 0;
    if (page_index != -1) {
        flowOnPageLoadedStudio(page_index);
    }
}

EM_PORT_API(void) lvglDeleteObject(lv_obj_t *obj) {
    if (obj == current_screen) {
        printf("delete current screen called, set fallback screen\n");
        static lv_obj_t *fallback_screen = 0;
        if (!fallback_screen) {
            fallback_screen = lv_obj_create(0);
        }
        lv_scr_load(fallback_screen);
        current_screen = fallback_screen;
    }
    lv_obj_del(obj);
}

EM_PORT_API(void) lvglObjAddFlag(lv_obj_t *obj, lv_obj_flag_t f) {
    lv_obj_add_flag(obj, f);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglObjClearFlag(lv_obj_t *obj, lv_obj_flag_t f) {
    lv_obj_clear_flag(obj, f);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglObjAddState(lv_obj_t *obj, lv_obj_flag_t s) {
    lv_obj_add_state(obj, s);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglObjClearState(lv_obj_t *obj, lv_obj_flag_t s) {
    lv_obj_clear_state(obj, s);
    lv_obj_update_layout(obj);
}

EM_PORT_API(uint32_t) lvglObjGetStylePropColor(lv_obj_t *obj, lv_part_t part, lv_style_prop_t prop) {
    lv_style_value_t value = lv_obj_get_style_prop(obj, part, prop);
    return value.color.full;
}

EM_PORT_API(int32_t) lvglObjGetStylePropNum(lv_obj_t *obj, lv_part_t part, lv_style_prop_t prop) {
    lv_style_value_t value = lv_obj_get_style_prop(obj, part, prop);
    return value.num;
}

EM_PORT_API(void) lvglObjSetLocalStylePropColor(lv_obj_t *obj, lv_style_prop_t prop, uint32_t color, lv_style_selector_t selector) {
    lv_style_value_t value;
    value.color = lv_color_hex(color);
    lv_obj_set_local_style_prop(obj, prop, value, selector);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglObjSetLocalStylePropNum(lv_obj_t *obj, lv_style_prop_t prop, int32_t num, lv_style_selector_t selector) {
    lv_style_value_t value;
    value.num = num;
    lv_obj_set_local_style_prop(obj, prop, value, selector);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglObjSetLocalStylePropPtr(lv_obj_t *obj, lv_style_prop_t prop, const void *ptr, lv_style_selector_t selector) {
    lv_style_value_t value;
    value.ptr = ptr;
    lv_obj_set_local_style_prop(obj, prop, value, selector);
    lv_obj_update_layout(obj);
}

static const lv_font_t *BUILT_IN_FONTS[] = {
    &lv_font_montserrat_8,
    &lv_font_montserrat_10,
    &lv_font_montserrat_12,
    &lv_font_montserrat_14,
    &lv_font_montserrat_16,
    &lv_font_montserrat_18,
    &lv_font_montserrat_20,
    &lv_font_montserrat_22,
    &lv_font_montserrat_24,
    &lv_font_montserrat_26,
    &lv_font_montserrat_28,
    &lv_font_montserrat_30,
    &lv_font_montserrat_32,
    &lv_font_montserrat_34,
    &lv_font_montserrat_36,
    &lv_font_montserrat_38,
    &lv_font_montserrat_40,
    &lv_font_montserrat_42,
    &lv_font_montserrat_44,
    &lv_font_montserrat_46,
    &lv_font_montserrat_48
};

EM_PORT_API(int32_t) lvglObjGetStylePropBuiltInFont(lv_obj_t *obj, lv_part_t part, lv_style_prop_t prop) {
    lv_style_value_t value = lv_obj_get_style_prop(obj, part, prop);
    for (uint32_t fontIndex = 0; fontIndex < sizeof(BUILT_IN_FONTS) / sizeof(lv_font_t *); fontIndex++) {
        if (value.ptr == BUILT_IN_FONTS[fontIndex]) {
            return (int32_t)fontIndex;
        }
    }
    return -1;
}

EM_PORT_API(const void *) lvglObjGetStylePropFontAddr(lv_obj_t *obj, lv_part_t part, lv_style_prop_t prop) {
    lv_style_value_t value = lv_obj_get_style_prop(obj, part, prop);
    return value.ptr;
}

EM_PORT_API(void) lvglObjSetLocalStylePropBuiltInFont(lv_obj_t *obj, lv_style_prop_t prop, int font_index, lv_style_selector_t selector) {
    lv_style_value_t value;
    value.ptr = BUILT_IN_FONTS[font_index];
    lv_obj_set_local_style_prop(obj, prop, value, selector);
    lv_obj_update_layout(obj);
}

EM_PORT_API(int16_t) lvglGetObjRelX(lv_obj_t *obj) {
    lv_obj_t *parent = lv_obj_get_parent(obj);
    if (parent) {
        return obj->coords.x1 - parent->coords.x1;
    }
    return obj->coords.x1;
}

EM_PORT_API(int16_t) lvglGetObjRelY(lv_obj_t *obj) {
    lv_obj_t *parent = lv_obj_get_parent(obj);
    if (parent) {
        return obj->coords.y1 - parent->coords.y1;
    }
    return obj->coords.y1;
}

EM_PORT_API(int16_t) lvglGetObjWidth(lv_obj_t *obj) {

    return lv_obj_get_width(obj);
}

EM_PORT_API(int16_t) lvglGetObjHeight(lv_obj_t *obj) {
    return lv_obj_get_height(obj);
}

EM_PORT_API(lv_font_t *) lvglLoadFont(const char *font_file_path) {
    return lv_font_load(font_file_path);
}

EM_PORT_API(void) lvglFreeFont(lv_font_t *font) {
    return lv_font_free(font);
}

EM_PORT_API(void) lvglAddObjectFlowCallback(lv_obj_t *obj, lv_event_code_t filter, void *flow_state, unsigned component_index, unsigned output_or_property_index) {
    FlowEventCallbackData *data = (FlowEventCallbackData *)lv_mem_alloc(sizeof(FlowEventCallbackData));

    data->flow_state = flow_state;
    data->component_index = component_index;
    data->output_or_property_index = output_or_property_index;

    if (filter == LV_EVENT_METER_TICK_LABEL_EVENT) {
        lv_obj_add_event_cb(obj, flow_event_meter_tick_label_event_callback, LV_EVENT_DRAW_PART_BEGIN, data);
    } else if (filter == LV_EVENT_DROPDOWN_SELECTED_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_dropdown_selected_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_ROLLER_SELECTED_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_roller_selected_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_TEXTAREA_TEXT_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_textarea_text_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_CHECKED_STATE_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_checked_state_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_ARC_VALUE_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_arc_value_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_SLIDER_VALUE_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_slider_value_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_SLIDER_VALUE_LEFT_CHANGED) {
        lv_obj_add_event_cb(obj, flow_event_slider_value_left_changed_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_CHECKED) {
        lv_obj_add_event_cb(obj, flow_event_checked_callback, LV_EVENT_VALUE_CHANGED, data);
    } else if (filter == LV_EVENT_UNCHECKED) {
        lv_obj_add_event_cb(obj, flow_event_unchecked_callback, LV_EVENT_VALUE_CHANGED, data);
    } else {
        lv_obj_add_event_cb(obj, flow_event_callback, filter, data);
    }

    lv_obj_add_event_cb(obj, flow_event_callback_delete_user_data, LV_EVENT_DELETE, data);
}

EM_PORT_API(void) lvglUpdateLabelText(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_LABEL_TEXT, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglSetImageSrc(lv_obj_t *obj, const void *img_src, lv_coord_t pivotX, lv_coord_t pivotY, uint16_t zoom, int16_t angle) {
    lv_img_set_src(obj, img_src);
    lv_img_set_pivot(obj, pivotX, pivotY);
    lv_img_set_zoom(obj, zoom);
    lv_img_set_angle(obj, angle);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglSetImgbuttonImageSrc(lv_obj_t *obj, lv_imgbtn_state_t state, const void *img_src) {
    lv_imgbtn_set_src(obj, state, NULL, img_src, NULL);
    lv_obj_update_layout(obj);
}

EM_PORT_API(void) lvglSetKeyboardTextarea(lv_obj_t *obj, lv_obj_t *textarea) {
    lv_keyboard_set_textarea(obj, textarea);
    lv_obj_update_layout(obj);
}

EM_PORT_API(lv_meter_scale_t *) lvglMeterAddScale(
    lv_obj_t *obj,
    uint16_t minorTickCount, uint16_t minorTickLineWidth, uint16_t minorTickLength, uint32_t minorTickColor,
    uint16_t nthMajor, uint16_t majorTickWidth, uint16_t majorTickLength, uint32_t majorTickColor, int16_t labelGap,
    int32_t scaleMin, int32_t scaleMax, uint32_t scaleAngleRange, uint32_t scaleRotation
) {
    lv_meter_scale_t *scale = lv_meter_add_scale(obj);
    lv_meter_set_scale_ticks(obj, scale, minorTickCount, minorTickLineWidth, minorTickLength, lv_color_hex(minorTickColor));
    lv_meter_set_scale_major_ticks(obj, scale, nthMajor, majorTickWidth, majorTickLength, lv_color_hex(majorTickColor), labelGap);
    lv_meter_set_scale_range(obj, scale, scaleMin, scaleMax, scaleAngleRange, scaleRotation);
    return scale;
}

EM_PORT_API(lv_meter_indicator_t *) lvglMeterAddIndicatorNeedleImg(lv_obj_t *obj, lv_meter_scale_t *scale, const void *img_src, lv_coord_t pivotX, lv_coord_t pivotY, int32_t value) {
    lv_meter_indicator_t *indicator = lv_meter_add_needle_img(obj, scale, img_src, pivotX, pivotY);
    lv_meter_set_indicator_value(obj, indicator, value);
    return indicator;
}

EM_PORT_API(lv_meter_indicator_t *) lvglMeterAddIndicatorNeedleLine(lv_obj_t *obj, lv_meter_scale_t *scale, uint16_t width, uint32_t color, int16_t radiusModifier, int32_t value) {
    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, width, lv_color_hex(color), radiusModifier);
    lv_meter_set_indicator_value(obj, indicator, value);
    return indicator;
}

EM_PORT_API(lv_meter_indicator_t *) lvglMeterAddIndicatorScaleLines(lv_obj_t *obj, lv_meter_scale_t *scale, uint32_t colorStart, uint32_t colorEnd, bool local, int16_t widthModifier, int32_t startValue, int32_t endValue) {
    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(colorStart), lv_color_hex(colorEnd), local, widthModifier);
    lv_meter_set_indicator_start_value(obj, indicator, startValue);
    lv_meter_set_indicator_end_value(obj, indicator, endValue);
    return indicator;
}

EM_PORT_API(lv_meter_indicator_t *) lvglMeterAddIndicatorArc(lv_obj_t *obj, lv_meter_scale_t *scale, uint16_t width, uint32_t color, int16_t radiusModifier, int32_t startValue, int32_t endValue) {
    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, width, lv_color_hex(color), radiusModifier);
    lv_meter_set_indicator_start_value(obj, indicator, startValue);
    lv_meter_set_indicator_end_value(obj, indicator, endValue);
    return indicator;
}

EM_PORT_API(void) lvglUpdateMeterIndicatorValue(lv_obj_t *obj, lv_meter_indicator_t *indicator, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_METER_INDICATOR_VALUE, obj, flow_state, component_index, property_index, indicator, 0);
}

EM_PORT_API(void) lvglUpdateMeterIndicatorStartValue(lv_obj_t *obj, lv_meter_indicator_t *indicator, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_METER_INDICATOR_START_VALUE, obj, flow_state, component_index, property_index, indicator, 0);
}

EM_PORT_API(void) lvglUpdateMeterIndicatorEndValue(lv_obj_t *obj, lv_meter_indicator_t *indicator, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_METER_INDICATOR_END_VALUE, obj, flow_state, component_index, property_index, indicator, 0);
}

EM_PORT_API(void) lvglUpdateDropdownOptions(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_DROPDOWN_OPTIONS, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateDropdownSelected(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_DROPDOWN_SELECTED, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateRollerOptions(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index, lv_roller_mode_t mode) {
    addUpdateTask(UPDATE_TASK_TYPE_ROLLER_OPTIONS, obj, flow_state, component_index, property_index, 0, mode);
}

EM_PORT_API(void) lvglUpdateRollerSelected(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_ROLLER_SELECTED, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateSliderValue(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_SLIDER_VALUE, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateSliderValueLeft(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_SLIDER_VALUE_LEFT, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateBarValue(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_BAR_VALUE, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateBarValueStart(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_BAR_VALUE_START, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateArcValue(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_ARC_VALUE, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateTextareaText(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_TEXTAREA_TEXT, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateCheckedState(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_CHECKED_STATE, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateDisabledState(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_DISABLED_STATE, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateHiddenFlag(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_HIDDEN_FLAG, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglUpdateClickableFlag(lv_obj_t *obj, void *flow_state, unsigned component_index, unsigned property_index) {
    addUpdateTask(UPDATE_TASK_TYPE_CLICKABLE_FLAG, obj, flow_state, component_index, property_index, 0, 0);
}

EM_PORT_API(void) lvglAddTimelineKeyframe(
    lv_obj_t *obj,
    void *flowState,
    float start, float end,
    uint32_t enabledProperties,
    int16_t x, uint8_t xEasingFunc,
    int16_t y, uint8_t yEasingFunc,
    int16_t width, uint8_t widthEasingFunc,
    int16_t height, uint8_t heightEasingFunc,
    int16_t opacity, uint8_t opacityEasingFunc,
    int16_t scale, uint8_t scaleEasingFunc,
    int16_t rotate, uint8_t rotateEasingFunc,
    int32_t cp1x, int32_t cp1y, int32_t cp2x, int32_t cp2y
) {
    addTimelineKeyframe(
        obj,
        flowState,
        start, end,
        enabledProperties,
        x, xEasingFunc,
        y, yEasingFunc,
        width, widthEasingFunc,
        height, heightEasingFunc,
        opacity, opacityEasingFunc,
        scale, scaleEasingFunc,
        rotate, rotateEasingFunc,
        cp1x, cp1y, cp2x, cp2y
    );
}

EM_PORT_API(void) lvglSetTimelinePosition(float timelinePosition) {
    setTimelinePosition(timelinePosition);
}

EM_PORT_API(void) lvglClearTimeline() {
    clearTimeline();
}

////////////////////////////////////////////////////////////////////////////////
