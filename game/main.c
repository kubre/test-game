typedef enum EntityType {
    Entity_NIL,
    Entity_SKILLET,
    Entity_TEA,
    Entity_TEA_POWDER,
    Entity_WATER,
    Entity_MILK,
    Entity_SUGAR,
    Entity_STOVE,
} EntityType;


typedef struct Entity {
    Gfx_Image* image;
    EntityType type;
    bool       is_valid;
    Vector2    pos;
} Entity;


#define MAX_ENTITIES_COUNT 1024
typedef struct World {
    Entity entities[MAX_ENTITIES_COUNT];
} World;
World* world = 0;


Entity* Entity_create() {
    Entity* entity_found = 0;
    for (int i = 0; i < MAX_ENTITIES_COUNT; ++i) {
        Entity* existing_entity = &world->entities[i];
        if (!existing_entity->is_valid) {
            entity_found = existing_entity;
            break;
        }
    }
    entity_found->is_valid = true;
    return entity_found;
}

void Entity_destroy(Entity* entity) {
    memset(entity, 0, sizeof(Entity));
}

int entry(int argc, char **argv) {

	window.title = STR("Pune");
	window.scaled_width = 1280; // We need to set the scaled size if we want to handle system scaling (DPI)
	window.scaled_height = 720;
	window.x = 200;
	window.y = 90;
	window.clear_color = hex_to_rgba(0x222034ff);

    Gfx_Image *skillet    = load_image_from_disk(
                                STR("sprites/skillet.png"), get_heap_allocator());
    Gfx_Image *milk       = load_image_from_disk(
                                STR("sprites/milk.png"), get_heap_allocator());
    Gfx_Image *stove      = load_image_from_disk(
                                STR("sprites/stove.png"), get_heap_allocator());
    Gfx_Image *sugar      = load_image_from_disk(
                                STR("sprites/sugar.png"), get_heap_allocator());
    Gfx_Image *tea_powder = load_image_from_disk(
                                STR("sprites/tea_powder.png"), get_heap_allocator());
    Gfx_Image *tea        = load_image_from_disk(
                                STR("sprites/tea.png"), get_heap_allocator());
    Gfx_Image *water      = load_image_from_disk(
                                STR("sprites/water.png"), get_heap_allocator());

    assert(skillet, "skillet.png cannot be loaded!");
    assert(milk, "milk.png cannot be loaded!");
    assert(stove, "stove.png cannot be loaded!");
    assert(sugar, "sugar.png cannot be loaded!");
    assert(tea_powder, "tea_powder.png cannot be loaded!");
    assert(tea, "tea.png cannot be loaded!");
    assert(water, "water.png cannot be loaded!");

    f32 zoom = 4.f;

    Vector2 last_pos = v2(0, 0);
	while (!window.should_close) {
		reset_temporary_storage();

        draw_frame.projection = m4_make_orthographic_projection(
                                    window.width * -.5f, window.width * .5f,
                                    window.height * -.5f, window.height * .5f,
                                    -1, 10);
        draw_frame.camera_xform = m4_make_scale(v3(1.0/zoom, 1.0/zoom, 1.0f));
		os_update();

        if (is_key_just_pressed(KEY_ESCAPE)) {
            window.should_close = true;
        }


        Vector2 mouse_pos = v2(
            (input_frame.mouse_x - (f32)window.width * .5f) / zoom,
            (input_frame.mouse_y - (f32)window.height * .5f) / zoom);
        Vector3 skillet_pos = v3((last_pos.x - (f32)skillet->width * .5f),
                                 (last_pos.y - (f32)skillet->height * .5f), 0);

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(-80, -40, 0));
            draw_image_xform(skillet, xform,
                             v2(skillet->width, skillet->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(-120, -40, 0));
            draw_image_xform(tea_powder, xform, v2(tea_powder->width, tea_powder->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(-120, -20, 0));
            draw_image_xform(sugar, xform, v2(sugar->width, sugar->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(-120, 0, 0));
            draw_image_xform(milk, xform, v2(milk->width, milk->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(20, 50, 0));
            draw_image_xform(tea, xform, v2(tea->width, tea->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(0, -20, 0));
            draw_image_xform(stove, xform, v2(stove->width, stove->height), COLOR_WHITE);
        }

        if (is_key_down(MOUSE_BUTTON_LEFT) &&
            (mouse_pos.x > skillet_pos.x) &&
            (mouse_pos.x < skillet_pos.x + (f32)skillet->width) &&
            (mouse_pos.y > skillet_pos.y) &&
            (mouse_pos.y < skillet_pos.y + (f32)skillet->height)) {
            skillet_pos = v3(mouse_pos.x - (f32)skillet->width * .5f,
                             mouse_pos.y - (f32)skillet->height * .5f, 0);
            last_pos = v2(mouse_pos.x, mouse_pos.y);
        }
		gfx_update();
	}

	return 0;
}