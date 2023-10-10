#include "hw1.h"
#include "hw1_scenes.h"

using namespace hw1;

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
            Real distanceSquared = dot(pixelCenter - center, pixelCenter - center);
            Real radiusSquared = radius * radius;
            
            if(distanceSquared <= radiusSquared){
                img(x, y) = color;
            }else{
                img(x, y) = Vector3{0.5, 0.5, 0.5};
            }
        }
    }
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render multiple circles
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    int scene_id = std::stoi(params[0]);
    const CircleScene &scene = hw1_2_scenes[scene_id];

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};

            Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
            Real numObjects = scene.objects.size();
            for (int i = 0; i < numObjects; i++) {
                Real distance = length(pixelCenter - scene.objects[i].center);

                if (distance <= scene.objects[i].radius) {
                    // Set the pixel's color to the circle's color
                    img(x, y) = scene.objects[i].color;
                }
            }
        }
    }

    // Real numObjects = scene.objects.size();
    // for (int i = 0; i < numObjects; i++) {
    //     Vector2 bounding_box_min = scene.objects[i].center - Vector2(scene.objects[i].radius, scene.objects[i].radius);
    //     Vector2 bounding_box_max = scene.objects[i].center + Vector2(scene.objects[i].radius, scene.objects[i].radius);
    //     for (int y = bounding_box_min.y; y < bounding_box_max.y; y++) {
    //         for (int x = bounding_box_min.x; x < bounding_box_max.x; x++) {
    //             img(x, y) = Vector3{0.5, 0.5, 0.5};

    //             Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
    //             Real distance = length(pixelCenter - scene.objects[i].center);

    //             if (distance <= scene.objects[i].radius) {
    //                 // Set the pixel's color to the circle's color
    //                 img(x, y) = scene.objects[i].color;
    //             }
    //         }
    //     }
    // }
    
    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            
            Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
            img(x, y) = scene.background;

            for (int i = 0; i < scene.shapes.size(); i++) {
                const Shape &shape = scene.shapes[i];

                if (auto *circle = std::get_if<Circle>(&shape)) {
                    Real distance = length(pixelCenter - circle->center);

                    if (distance <= circle->radius) {
                        // Set the pixel's color to the circle's color
                        img(x, y) = circle->color;
                    }
                } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                    if (pixelCenter.x >= rectangle->p_min.x &&
                        pixelCenter.x <= rectangle->p_max.x &&
                        pixelCenter.y >= rectangle->p_min.y &&
                        pixelCenter.y <= rectangle->p_max.y) {
                        img(x, y) = rectangle->color;
                    }
                } else if (auto *triangle = std::get_if<Triangle>(&shape)) {
                    Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                    Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                    Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                    Vector2 e01 = p1 - p0;
                    Vector2 e12 = p2 - p1;
                    Vector2 e20 = p0 - p2;

                    Vector2 n01(e01.y, -e01.x);
                    Vector2 n12(e12.y, -e12.x);
                    Vector2 n20(e20.y, -e20.x);

                    Vector2 v0 = p0 - pixelCenter;
                    Vector2 v1 = p1 - pixelCenter;
                    Vector2 v2 = p2 - pixelCenter;
                    
                    if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                        img(x, y) = triangle->color;
                    }
                }
            }
        }
    }
    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            
            Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
            img(x, y) = scene.background;

            for (int i = 0; i < scene.shapes.size(); i++) {
                const Shape &shape = scene.shapes[i];

                // // Scale
                //     if((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                //         if (auto *circle = std::get_if<Circle>(&shape)) {
                //             Matrix3x3 transform_type = circle->transform;
                //             Real distance = length(pixelCenter - circle->center);

                //         } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                            
                //             Matrix3x3 transform_type = rectangle->transform;
                            
                //         } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                //             Matrix3x3 transform_type = triangle->transform;

                //             Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                //             Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                //             Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                //             Vector2 e01 = p1 - p0;
                //             Vector2 e12 = p2 - p1;
                //             Vector2 e20 = p0 - p2;

                //             Vector2 n01(e01.y, -e01.x);
                //             Vector2 n12(e12.y, -e12.x);
                //             Vector2 n20(e20.y, -e20.x);

                //             Vector2 v0 = p0 - pixelCenter;
                //             Vector2 v1 = p1 - pixelCenter;
                //             Vector2 v2 = p2 - pixelCenter;
                //         }

                //     // Shear_x
                //     } else if ((transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                        
                //         if (auto *circle = std::get_if<Circle>(&shape)) {
                //             Matrix3x3 transform_type = circle->transform;
                //             Real distance = length(pixelCenter - circle->center);

                //             if (distance <= circle->radius) {
                //                 // Set the pixel's color to the circle's color
                //                 img(x + transform_type(0,1), y) = circle->color;
                //             }

                //         } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                            
                //             Matrix3x3 transform_type = rectangle->transform;
                            
                //         } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                //             Matrix3x3 transform_type = triangle->transform;

                //             Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                //             Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                //             Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                //             Vector2 e01 = p1 - p0;
                //             Vector2 e12 = p2 - p1;
                //             Vector2 e20 = p0 - p2;

                //             Vector2 n01(e01.y, -e01.x);
                //             Vector2 n12(e12.y, -e12.x);
                //             Vector2 n20(e20.y, -e20.x);

                //             Vector2 v0 = p0 - pixelCenter;
                //             Vector2 v1 = p1 - pixelCenter;
                //             Vector2 v2 = p2 - pixelCenter;
                //         }
                        
                //     // Shear_y
                //     } else if ((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                //         if (auto *circle = std::get_if<Circle>(&shape)) {
                //             Matrix3x3 transform_type = circle->transform;
                //             Real distance = length(pixelCenter - circle->center);

                //         } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                            
                //             Matrix3x3 transform_type = rectangle->transform;
                            
                //         } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                //             Matrix3x3 transform_type = triangle->transform;

                //             Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                //             Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                //             Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                //             Vector2 e01 = p1 - p0;
                //             Vector2 e12 = p2 - p1;
                //             Vector2 e20 = p0 - p2;

                //             Vector2 n01(e01.y, -e01.x);
                //             Vector2 n12(e12.y, -e12.x);
                //             Vector2 n20(e20.y, -e20.x);

                //             Vector2 v0 = p0 - pixelCenter;
                //             Vector2 v1 = p1 - pixelCenter;
                //             Vector2 v2 = p2 - pixelCenter;
                //         }
                        
                //     // Rotate
                //     }else if ((transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                //         if (auto *circle = std::get_if<Circle>(&shape)) {
                //             Matrix3x3 transform_type = circle->transform;
                //             Real distance = length(pixelCenter - circle->center);

                //         } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                            
                //             Matrix3x3 transform_type = rectangle->transform;
                            
                //         } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                //             Matrix3x3 transform_type = triangle->transform;

                //             Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                //             Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                //             Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                //             Vector2 e01 = p1 - p0;
                //             Vector2 e12 = p2 - p1;
                //             Vector2 e20 = p0 - p2;

                //             Vector2 n01(e01.y, -e01.x);
                //             Vector2 n12(e12.y, -e12.x);
                //             Vector2 n20(e20.y, -e20.x);

                //             Vector2 v0 = p0 - pixelCenter;
                //             Vector2 v1 = p1 - pixelCenter;
                //             Vector2 v2 = p2 - pixelCenter;
                //         }

                //     // Translate
                //     } else if ((transform_type(0,1) == 0) && (transform_type(1,0) == 0) && (transform_type(2,0) == 0)){
                //         if (auto *circle = std::get_if<Circle>(&shape)) {
                //             Matrix3x3 transform_type = circle->transform;
                //             Real distance = length(pixelCenter - circle->center);

                //         } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                            
                //             Matrix3x3 transform_type = rectangle->transform;
                            
                //         } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                //             Matrix3x3 transform_type = triangle->transform;

                //             Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                //             Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                //             Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                //             Vector2 e01 = p1 - p0;
                //             Vector2 e12 = p2 - p1;
                //             Vector2 e20 = p0 - p2;

                //             Vector2 n01(e01.y, -e01.x);
                //             Vector2 n12(e12.y, -e12.x);
                //             Vector2 n20(e20.y, -e20.x);

                //             Vector2 v0 = p0 - pixelCenter;
                //             Vector2 v1 = p1 - pixelCenter;
                //             Vector2 v2 = p2 - pixelCenter;
                //         }
                //     }
                
                if (auto *circle = std::get_if<Circle>(&shape)) {
                    Matrix3x3 transform_type = circle->transform;
                    Real distance = length(pixelCenter - circle->center);

                    // Scale
                    if((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if (distance <= circle->radius) {
                            // Set the pixel's color to the circle's color
                            img(x * transform_type(0,0), y * transform_type(1,1)) = circle->color;
                        }

                    // Shear_x
                    } else if ((transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                        
                        if (distance <= circle->radius) {
                            // Set the pixel's color to the circle's color
                            img(x + transform_type(0,1), y) = circle->color;
                        }
                        
                    // Shear_y
                    } else if ((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if (distance <= circle->radius) {
                            // Set the pixel's color to the circle's color
                            img(x, y + transform_type(1,0)) = circle->color;
                        }
                        
                    // Rotate
                    }else if ((transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                        // Calculate the offset from the circle's center to the current pixel
                        Vector2 offset(x - circle->center.x, y - circle->center.y);

                        // Apply the rotation transformation to the offset
                        Real angleRadians = acos(transform_type(0, 0));  // Assuming transform_type(0, 0) stores the rotation angle in radians
                        Vector2 rotatedOffset;
                        rotatedOffset.x = offset.x * cos(angleRadians) - offset.y * sin(angleRadians);
                        rotatedOffset.y = offset.x * sin(angleRadians) + offset.y * cos(angleRadians);

                        // Calculate the rotated pixel coordinates
                        int rotatedX = circle->center.x + rotatedOffset.x;
                        int rotatedY = circle->center.y + rotatedOffset.y;

                        if (distance <= circle->radius && rotatedX >= 0 && rotatedX < img.width && rotatedY >= 0 && rotatedY < img.height) {
                            // Set the pixel's color to the circle's color
                            img(rotatedX, rotatedY) = circle->color;
                        }

                    // Translate
                    } else if ((transform_type(0,1) == 0) && (transform_type(1,0) == 0) && (transform_type(2,0) == 0)){
                        if (distance <= circle->radius) {
                            // Set the pixel's color to the circle's color
                            img(x + transform_type(0,2), y + transform_type(1,2)) = circle->color;
                        }
                    }





                } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                    
                    Matrix3x3 transform_type = rectangle->transform;

                    // Scale
                    if((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                        if (pixelCenter.x >= rectangle->p_min.x &&
                            pixelCenter.x <= rectangle->p_max.x &&
                            pixelCenter.y >= rectangle->p_min.y &&
                            pixelCenter.y <= rectangle->p_max.y) {
                            img(x * transform_type(0,0), y * transform_type(1,1)) = rectangle->color;
                        }

                    // Shear_x
                    } else if ((transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if (pixelCenter.x >= rectangle->p_min.x &&
                            pixelCenter.x <= rectangle->p_max.x &&
                            pixelCenter.y >= rectangle->p_min.y &&
                            pixelCenter.y <= rectangle->p_max.y) {
                            img(x + transform_type(0,1), y) = rectangle->color;
                        }
                        
                    // Shear_y
                    } else if ((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if (pixelCenter.x >= rectangle->p_min.x &&
                            pixelCenter.x <= rectangle->p_max.x &&
                            pixelCenter.y >= rectangle->p_min.y &&
                            pixelCenter.y <= rectangle->p_max.y) {
                            img(x, y + transform_type(1,0)) = rectangle->color;
                        }
                        
                    // Rotate
                    }else if ((transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        // Calculate the offset from the circle's center to the current pixel
                        Vector2 offset(x - rectangle->p_min.x, y - rectangle->p_min.y);

                        // Apply the rotation transformation to the offset
                        Real angleRadians = acos(transform_type(0, 0));  // Assuming transform_type(0, 0) stores the rotation angle in radians
                        Vector2 rotatedOffset;
                        rotatedOffset.x = offset.x * cos(angleRadians) - offset.y * sin(angleRadians);
                        rotatedOffset.y = offset.x * sin(angleRadians) + offset.y * cos(angleRadians);

                        // Calculate the rotated pixel coordinates
                        int rotatedX = rectangle->p_min.x + rotatedOffset.x;
                        int rotatedY = rectangle->p_min.y + rotatedOffset.y;

                        if (pixelCenter.x >= rectangle->p_min.x &&
                            pixelCenter.x <= rectangle->p_max.x &&
                            pixelCenter.y >= rectangle->p_min.y &&
                            pixelCenter.y <= rectangle->p_max.y &&
                            rotatedX >= 0 && rotatedX < img.width && rotatedY >= 0 && rotatedY < img.height) {
                            // Set the pixel's color to the circle's color
                            img(rotatedX, rotatedY) = rectangle->color;
                        }
                        
                    // Translate
                    } else if ((transform_type(0,1) == 0) && (transform_type(1,0) == 0) && (transform_type(2,0) == 0)){
                        
                        if (pixelCenter.x >= rectangle->p_min.x &&
                            pixelCenter.x <= rectangle->p_max.x &&
                            pixelCenter.y >= rectangle->p_min.y &&
                            pixelCenter.y <= rectangle->p_max.y) {
                                img(x + transform_type(0,2), y + transform_type(1,2)) = rectangle->color;
                            }

                    }



                    
                } else if (auto *triangle = std::get_if<Triangle>(&shape)) {

                    Matrix3x3 transform_type = triangle->transform;

                    Vector2 p0 = triangle->p0 + Vector2(0.5, 0.5);
                    Vector2 p1 = triangle->p1 + Vector2(0.5, 0.5);
                    Vector2 p2 = triangle->p2 + Vector2(0.5, 0.5);

                    Vector2 e01 = p1 - p0;
                    Vector2 e12 = p2 - p1;
                    Vector2 e20 = p0 - p2;

                    Vector2 n01(e01.y, -e01.x);
                    Vector2 n12(e12.y, -e12.x);
                    Vector2 n20(e20.y, -e20.x);

                    Vector2 v0 = p0 - pixelCenter;
                    Vector2 v1 = p1 - pixelCenter;
                    Vector2 v2 = p2 - pixelCenter;

                    // Scale
                    if((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){
                        
                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                            img(x * transform_type(0,0), y * transform_type(1,1)) = triangle->color;
                        }

                    // Shear_x
                    } else if ((transform_type(0,2) == 0) && (transform_type(1,0) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                            img(x + transform_type(0,1), y) = triangle->color;
                        }
                        
                    // Shear_y
                    } else if ((transform_type(0,1) == 0) && (transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                            img(x, y + transform_type(1,0)) = triangle->color;
                        }
                        
                    // Rotate
                    }else if ((transform_type(0,2) == 0) && (transform_type(1,2) == 0) && (transform_type(2,0) == 0)){

                        // Calculate the offset from the circle's center to the current pixel
                        Vector2 offset(x - triangle->p0.x, y - triangle->p0.y);

                        // Apply the rotation transformation to the offset
                        Real angleRadians = acos(transform_type(0, 0));  // Assuming transform_type(0, 0) stores the rotation angle in radians
                        Vector2 rotatedOffset;
                        rotatedOffset.x = offset.x * cos(angleRadians) - offset.y * sin(angleRadians);
                        rotatedOffset.y = offset.x * sin(angleRadians) + offset.y * cos(angleRadians);

                        // Calculate the rotated pixel coordinates
                        int rotatedX = triangle->p0.x + rotatedOffset.x;
                        int rotatedY = triangle->p0.y + rotatedOffset.y;

                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0) && rotatedX >= 0 && rotatedX < img.width && rotatedY >= 0 && rotatedY < img.height) {
                            // Set the pixel's color to the circle's color
                            img(rotatedX, rotatedY) = triangle->color;
                        }
                        
                    // Translate
                    } else if ((transform_type(0,1) == 0) && (transform_type(1,0) == 0) && (transform_type(2,0) == 0)){
                        
                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                            img(x + transform_type(0,2), y + transform_type(1,2)) = triangle->color;
                        }
                    }
                }
            }
        }
    }
    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}
