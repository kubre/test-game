typedef enum EntityType {
    Entity_NIL        = 0,
    Entity_SKILLET    = 1,
    Entity_TEA        = 2,
    Entity_TEA_POWDER = 3,
    Entity_WATER      = 4,
    Entity_MILK       = 5,
    Entity_SUGAR      = 6,
    Entity_STOVE      = 7,
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


bool is_v2_in_range(Vector2 point, Vector2 top_left, Vector2 bottom_right) {
    return (
        point.x >= top_left.x &&
        point.x <= bottom_right.x &&
        point.y >= top_left.y &&
        point.y <= bottom_right.y
    );
}

int entry(int argc, char **argv) {

	window.title = STR("Pune");
	window.scaled_width = 1280; // We need to set the scaled size if we want to handle system scaling (DPI)
	window.scaled_height = 720;
	window.x = 200;
	window.y = 90;
	window.clear_color = hex_to_rgba(0x222034ff);

    world = alloc(get_heap_allocator(), sizeof(World));

    Entity* skillet = Entity_create();
    skillet->image  = load_image_from_disk(STR("sprites/skillet.png"), get_heap_allocator());
    skillet->pos    = v2(-80.f, -40.f);
    skillet->type   = Entity_SKILLET;

    Entity* milk = Entity_create();
    milk->image  = load_image_from_disk(STR("sprites/milk.png"), get_heap_allocator());
    milk->pos    = v2(-120.f, 0.f);
    milk->type   = Entity_MILK;

    Entity* stove = Entity_create();
    stove->image  = load_image_from_disk(STR("sprites/stove.png"), get_heap_allocator());
    stove->pos    = v2(0.f, -20.f);
    stove->type   = Entity_STOVE;

    Entity* sugar = Entity_create();
    sugar->image  = load_image_from_disk(STR("sprites/sugar.png"), get_heap_allocator());
    sugar->pos    = v2(-120.f, -20.f);
    sugar->type   = Entity_SUGAR;

    Entity* tea_powder = Entity_create();
    tea_powder->image  = load_image_from_disk(STR("sprites/tea_powder.png"), get_heap_allocator());
    tea_powder->pos    = v2(-120.f, -40.f);
    tea_powder->type   = Entity_TEA_POWDER;

    Entity* tea = Entity_create();
    tea->image  = load_image_from_disk(STR("sprites/tea.png"), get_heap_allocator());
    tea->pos    = v2(20.f, 50.f);
    tea->type   = Entity_TEA;

    // Entity* water = Entity_create();
    // water->image  = load_image_from_disk(STR("sprites/water.png"), get_heap_allocator());

    assert(skillet, "skillet.png cannot be loaded!");
    assert(milk, "milk.png cannot be loaded!");
    assert(stove, "stove.png cannot be loaded!");
    assert(sugar, "sugar.png cannot be loaded!");
    assert(tea_powder, "tea_powder.png cannot be loaded!");
    assert(tea, "tea.png cannot be loaded!");
    // assert(water, "water.png cannot be loaded!");

    f32 zoom = 4.f;

    Entity* selected_entity = 0;

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


        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(stove->pos.x, stove->pos.y, 0));
            draw_image_xform(stove->image, xform, v2(stove->image->width, stove->image->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(skillet->pos.x, skillet->pos.y, 0));
            draw_image_xform(skillet->image, xform,
                             v2(skillet->image->width, skillet->image->height), COLOR_WHITE);
        }


        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(tea_powder->pos.x, tea_powder->pos.y, 0));
            draw_image_xform(tea_powder->image, xform, v2(tea_powder->image->width, tea_powder->image->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(sugar->pos.x, sugar->pos.y, 0));
            draw_image_xform(sugar->image, xform, v2(sugar->image->width, sugar->image->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(milk->pos.x, milk->pos.y, 0));
            draw_image_xform(milk->image, xform, v2(milk->image->width, milk->image->height), COLOR_WHITE);
        }

        {
            Matrix4 xform = m4_scalar(1.0);
            xform = m4_translate(xform, v3(tea->pos.x, tea->pos.y, 0));
            draw_image_xform(tea->image, xform, v2(tea->image->width, tea->image->height), COLOR_WHITE);
        }

        Vector2 mouse_pos = v2(
            (input_frame.mouse_x - (f32)window.width * .5f) / zoom,
            (input_frame.mouse_y - (f32)window.height * .5f) / zoom);
        if (is_key_down(MOUSE_BUTTON_LEFT)) {
            if (!selected_entity) { 
                for (int i = 0; i < MAX_ENTITIES_COUNT; ++i) {
                    Entity* entity_i = &world->entities[i];
                    if (!entity_i->is_valid) { continue; }

                    if (is_v2_in_range(mouse_pos, entity_i->pos,
                                       v2(entity_i->pos.x + (f32)entity_i->image->width,
                                          entity_i->pos.y + (f32)entity_i->image->height))) {
                            printf("Entity Selected %d\n", entity_i->type);
                            selected_entity = entity_i;
                            break;
                    }
                }
            }

            // not else cond cause we might select entity there wasnt one before
            if(selected_entity) {
                selected_entity->pos = v2(mouse_pos.x - (f32)selected_entity->image->width * .5f,
                                          mouse_pos.y - (f32)selected_entity->image->height * .5f);
            }
        }

        if (is_key_just_released(MOUSE_BUTTON_LEFT)) {
            selected_entity = 0;
        }
		gfx_update();
	}


    for (int i = 0; i < MAX_ENTITIES_COUNT; ++i) {
        Entity_destroy(&world->entities[i]);
    }

	return 0;
}