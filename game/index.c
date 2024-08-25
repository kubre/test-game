int entry(int argc, char **argv) {

	window.title = STR("Pune");
	window.scaled_width = 1280; // We need to set the scaled size if we want to handle system scaling (DPI)
	window.scaled_height = 720;
	window.x = 200;
	window.y = 90;
	window.clear_color = hex_to_rgba(0x222034ff);

    Gfx_Image* skillet = load_image_from_disk(fixed_string("sprites/skillet.png"), get_heap_allocator());

    f32 zoom = 4.f;

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

        Matrix4 skillet_xform = m4_scalar(1.0);
        Vector3 skillet_pos = v3(mouse_pos.x - skillet->width * .5f,
                                 mouse_pos.y - skillet->height * .5f,
                                 0);
        skillet_xform = m4_translate(skillet_xform, skillet_pos);
        draw_image_xform(skillet, skillet_xform,
                         v2(skillet->width, skillet->height), COLOR_WHITE);


		gfx_update();
	}

	return 0;
}