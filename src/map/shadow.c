
#include "shadow.h"

static ray_collision_t shadow_raycast(v2i p, double angle) {
    
    double cosa = cos(angle);
    double sina = -sin(angle);
    ray_collision_t collision = (ray_collision_t) { 0, 0, INFINITY };
    ray_collision_t new_collision;
    
    if(absd(cosa) > M_DELTA) {
        double m = sina / cosa;
        double c = p.y - m * p.x;
        // check edges of the map
        // top
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                (v2i) { 0, 0 }, (v2i) { RENDER.width, 0 });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                (v2i) { RENDER.width, 0 }, (v2i) { RENDER.width, RENDER.height });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                (v2i) { RENDER.width, RENDER.height }, (v2i) { 0, RENDER.height });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                (v2i) { 0, RENDER.height }, (v2i) { 0, 0 });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = __raycast(p, m, c, cosa, sina, game_data.map);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    } else {
        // check edges of the map
        // top
        new_collision = __ray_intersect_v(p, sina,
                (v2i) { 0, 0 }, (v2i) { RENDER.width, 0 });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect_v(p, sina,
                (v2i) { RENDER.width, 0 }, (v2i) { RENDER.width, RENDER.height });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect_v(p, sina,
                (v2i) { RENDER.width, RENDER.height }, (v2i) { 0, RENDER.height });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect_v(p, sina,
                (v2i) { 0, RENDER.height }, (v2i) { 0, 0 });
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = __raycast_v(p, sina, game_data.map);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

int sorter(list_data_t data1, list_data_t data2) {
    return data1.dvalue >= data2.dvalue ? 1 : -1;
}

void map_shadow(v2i p) {
    // draw to the framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, RENDER.mask.framebuffer);
    // glClearColor(255, 0, 0, 255);
    
    int i;
    list_t *angles = list_init();
    for(i = 0; i < game_data.map->n_points; i++) {
        double angle = atan2(p.y - game_data.map->points[i].y,
                game_data.map->points[i].x - p.x);
        list_add(angles, (list_data_t)angle_sanify(angle - SHADOW_DELTA));
        list_add(angles, (list_data_t)angle_sanify(angle));
        list_add(angles, (list_data_t)angle_sanify(angle + SHADOW_DELTA));
    }
    // check edges of the screen
    list_add(angles, (list_data_t)angle_sanify(atan2(p.y, -p.x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(p.y, RENDER.width - p.x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(p.y - RENDER.height, RENDER.width - p.x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(p.y - RENDER.height, -p.x)));
    list_quicksort_f(angles, sorter);
    
    int size = angles->size + 3;
    GLfloat data[size * 2];
    i = 0;
    
    data[i++] = game_to_gl_x(game_data.player.pos.x); data[i++] = game_to_gl_y(game_data.player.pos.y);
    
    double angle = list_remove(angles).dvalue;
    ray_collision_t first = shadow_raycast(p, angle);
    
    data[i++] = game_to_gl_x(first.x); data[i++] = game_to_gl_y(first.y);
    
    while(angles->size > 0) {
        double angle = list_remove(angles).dvalue;
        // printf("%f\n", angle);
        ray_collision_t collision = shadow_raycast(p, angle);
        // double px = collision.x;
        // double py = collision.y;
//        if(angle > M_PI_2 && angle < M_PI_2 * 3) px += 2;
//        else px -= 2;
//        if(angle > M_PI) py -= 2;
//        else py += 2;
        
        data[i++] = game_to_gl_x(collision.x); data[i++] = game_to_gl_y(collision.y);
    }
    list_free(angles);
    
    data[i++] = game_to_gl_x(first.x); data[i++] = game_to_gl_y(first.y);
    data[i++] = game_to_gl_x(game_data.player.pos.x); data[i++] = game_to_gl_y(game_data.player.pos.y);
    
    //glDisable(GL_BLEND);
    
    glUseProgram(RENDER.shaders.shadow);
    
    glBindVertexArray(RENDER.mask.vertex_array);
    
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.mask.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.mask.vertex_buffer);
    glVertexAttribPointer(
                    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    2, // size
                    GL_FLOAT, // type
                    GL_FALSE, // normalized?
                    0, // stride
                    NULL // array buffer offset
                    );
    glDrawArrays(GL_TRIANGLE_FAN, 0, size);
    glDisableVertexAttribArray(0);
    
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(2, GL_FLOAT, 0, data);
//    glDrawArrays(GL_POLYGON, 0, size);
//    glDisableClientState(GL_VERTEX_ARRAY);
    
    //glEnable(GL_BLEND);
    
    /*
    glColor3ub(0,255,0);
    glViewport(0, 0, RENDER.width, RENDER.height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // draw to the screen
    GLuint texID = glGetUniformLocation(RENDER.mask.program, "renderedTexture");
    glUseProgram(RENDER.mask.program);
    // Bind our texture in Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RENDER.mask.texture);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.mask.vertex_buffer);
    glVertexAttribPointer(
                    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3, // size
                    GL_FLOAT, // type
                    GL_FALSE, // normalized?
                    0, // stride
                    (void*) 0 // array buffer offset
                    );
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    glDisableVertexAttribArray(0);
    glUseProgram(0);
     */
}