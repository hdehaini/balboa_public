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
            img(x, y) = scene.background;
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;

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
    

    // ================================================================
    // BONUS: Use bounding boxes to speed up rendering
    // ================================================================

    Real numObjects = scene.objects.size();
    for (int i = 0; i < numObjects; i++) {
        const Circle &circle = scene.objects[i];

        Vector2 bounding_box_min = circle.center - Vector2(circle.radius, circle.radius);
        Vector2 bounding_box_max = circle.center + Vector2(circle.radius, circle.radius);

        for (int y = bounding_box_min.y; y < bounding_box_max.y; y++) {
            for (int x = bounding_box_min.x; x < bounding_box_max.x; x++) {
                // img(x, y) = scene.background;

                Vector2 pixelCenter(x + Real(0.5), y + Real(0.5));
                Real distance = length(pixelCenter - circle.center);

                if (distance <= circle.radius) {
                    // Set the pixel's color to the circle's color
                    img(x, y) = circle.color;
                }
            }
        }
    }
    
    return img;

    // ================================================================
    // ================================================================
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

            for (const Shape &shape : scene.shapes) {

                Matrix3x3 transform = get_transform(shape);
                Matrix3x3 inverseTransform = inverse(transform);

                Vector3 pixelCenter3D = Vector3(pixelCenter.x, pixelCenter.y, Real(1));

                // Apply the transformation to the pixel's center
                Vector3 objectSpaceCenter = inverseTransform * pixelCenter3D;

                Vector2 objectSpaceCenter2D = Vector2(objectSpaceCenter.x, objectSpaceCenter.y);
                

                if (auto *circle = std::get_if<Circle>(&shape)) {

                    Real distance = length(objectSpaceCenter2D - circle->center);

                    if (distance <= circle->radius) {
                        // Set the pixel's color to the circle's color
                        img(x, y) = circle->color;
                    }
                    
                } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {

                   // Check if the transformed pixel center is inside the transformed rectangle
                    if (objectSpaceCenter2D.x >= rectangle->p_min.x &&
                        objectSpaceCenter2D.x <= rectangle->p_max.x &&
                        objectSpaceCenter2D.y >= rectangle->p_min.y &&
                        objectSpaceCenter2D.y <= rectangle->p_max.y) {
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

                    Vector2 v0 = p0 - objectSpaceCenter2D;
                    Vector2 v1 = p1 - objectSpaceCenter2D;
                    Vector2 v2 = p2 - objectSpaceCenter2D;
                    
                    if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                        img(x, y) = triangle->color;
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

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            Vector3 subpixelColor = scene.background;
            Vector3 pixelColor = scene.background;
            img(x, y) = scene.background;

            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);

                    Vector2 pixelCenter(subpixelX, subpixelY);
                    

                    for (const Shape &shape : scene.shapes) {

                        Matrix3x3 transform = get_transform(shape);
                        Matrix3x3 inverseTransform = inverse(transform);

                        Vector3 pixelCenter3D = Vector3(pixelCenter.x, pixelCenter.y, Real(1));

                        // Apply the transformation to the pixel's center
                        Vector3 objectSpaceCenter = inverseTransform * pixelCenter3D;

                        Vector2 objectSpaceCenter2D = Vector2(objectSpaceCenter.x, objectSpaceCenter.y);

                        if (auto *circle = std::get_if<Circle>(&shape)) {

                            Real distance = length(objectSpaceCenter2D - circle->center);

                            if (distance <= circle->radius) {
                                subpixelColor = circle->color;
                            }

                        } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {

                        // Check if the transformed pixel center is inside the transformed rectangle
                            if (objectSpaceCenter2D.x >= rectangle->p_min.x &&
                                objectSpaceCenter2D.x <= rectangle->p_max.x &&
                                objectSpaceCenter2D.y >= rectangle->p_min.y &&
                                objectSpaceCenter2D.y <= rectangle->p_max.y) {
                                subpixelColor = rectangle->color;
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

                            Vector2 v0 = p0 - objectSpaceCenter2D;
                            Vector2 v1 = p1 - objectSpaceCenter2D;
                            Vector2 v2 = p2 - objectSpaceCenter2D;
                            
                            if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                subpixelColor = triangle->color;
                            }
                        }
                    }
                    pixelColor += subpixelColor;
                }
            }
            pixelColor /= Real(16);
            img(x, y) = pixelColor;
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

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            Vector3 subpixelColor = scene.background;
            Vector3 pixelColor = scene.background;
            img(x, y) = scene.background;

            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Real accum_alpha = 0;

                    Vector2 pixelCenter(subpixelX, subpixelY);
                    

                    for (const Shape &shape : scene.shapes) {

                        Matrix3x3 transform = get_transform(shape);
                        Matrix3x3 inverseTransform = inverse(transform);

                        Vector3 pixelCenter3D = Vector3(pixelCenter.x, pixelCenter.y, Real(1));

                        // Apply the transformation to the pixel's center
                        Vector3 objectSpaceCenter = inverseTransform * pixelCenter3D;

                        Vector2 objectSpaceCenter2D = Vector2(objectSpaceCenter.x, objectSpaceCenter.y);

                        if (auto *circle = std::get_if<Circle>(&shape)) {

                            Real distance = length(objectSpaceCenter2D - circle->center);
                            Real alpha = circle->alpha;
                            Vector3 objectColor = circle->color;
                            if (distance <= circle->radius) {
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }

                        } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {

                        // Check if the transformed pixel center is inside the transformed rectangle
                            if (objectSpaceCenter2D.x >= rectangle->p_min.x &&
                                objectSpaceCenter2D.x <= rectangle->p_max.x &&
                                objectSpaceCenter2D.y >= rectangle->p_min.y &&
                                objectSpaceCenter2D.y <= rectangle->p_max.y) {
                                
                                Real alpha = rectangle->alpha;
                                Vector3 objectColor = rectangle->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
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

                            Vector2 v0 = p0 - objectSpaceCenter2D;
                            Vector2 v1 = p1 - objectSpaceCenter2D;
                            Vector2 v2 = p2 - objectSpaceCenter2D;
                            
                            if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                Real alpha = triangle->alpha;
                                Vector3 objectColor = triangle->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }
                        }
                    }
                    pixelColor += subpixelColor;
                }
            }
            pixelColor /= Real(16);
            img(x, y) = pixelColor;
        }
    }
    return img;
}

Image3 hw_1_8(const std::vector<std::string> &params) {

    // Homework 1.8: render lines and quadratic/cubic Bézier curves
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            Vector3 subpixelColor = scene.background;
            Vector3 pixelColor = scene.background;
            img(x, y) = scene.background;

            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Real accum_alpha = 0;

                    Vector2 pixelCenter(subpixelX, subpixelY);
                    

                    for (const Shape &shape : scene.shapes) {

                        Matrix3x3 transform = get_transform(shape);
                        Matrix3x3 inverseTransform = inverse(transform);

                        Vector3 pixelCenter3D = Vector3(pixelCenter.x, pixelCenter.y, Real(1));

                        // Apply the transformation to the pixel's center
                        Vector3 objectSpaceCenter = inverseTransform * pixelCenter3D;

                        Vector2 objectSpaceCenter2D = Vector2(objectSpaceCenter.x, objectSpaceCenter.y);

                        if (auto *circle = std::get_if<Circle>(&shape)) {

                            Real distance = length(objectSpaceCenter2D - circle->center);
                            Real alpha = circle->alpha;
                            Vector3 objectColor = circle->color;
                            if (distance <= circle->radius) {
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }

                        } else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {

                        // Check if the transformed pixel center is inside the transformed rectangle
                            if (objectSpaceCenter2D.x >= rectangle->p_min.x &&
                                objectSpaceCenter2D.x <= rectangle->p_max.x &&
                                objectSpaceCenter2D.y >= rectangle->p_min.y &&
                                objectSpaceCenter2D.y <= rectangle->p_max.y) {
                                
                                Real alpha = rectangle->alpha;
                                Vector3 objectColor = rectangle->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
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

                            Vector2 v0 = p0 - objectSpaceCenter2D;
                            Vector2 v1 = p1 - objectSpaceCenter2D;
                            Vector2 v2 = p2 - objectSpaceCenter2D;
                            
                            if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                Real alpha = triangle->alpha;
                                Vector3 objectColor = triangle->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }
                        } else if (auto *line = std::get_if<Line>(&shape)) {

                            Vector2 p0 = line->start;
                            Vector2 p1 = line->end;
                            
                            // Calculate the distance from the point to the line defined by p0 and p1.
                            Real distance = std::abs(
                                (p1.y - p0.y) * subpixelX - (p1.x - p0.x) * subpixelY + p1.x * p0.y - p1.y * p0.x
                            ) / length(Vector2(p1.x - p0.x, p1.y - p0.y));
                            
                            // Check if the distance is within the line's width.
                            if (distance <= line->width * 0.5) {
                                Real alpha = line->alpha;
                                Vector3 objectColor = line->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }
                            
                        } else if (auto *quadratic = std::get_if<Quadratic>(&shape)) {
                            Vector2 p0 = quadratic->start;
                            Vector2 control = quadratic->control;
                            Vector2 p1 = quadratic->end;

                            // Calculate the distance from the object space center to the quadratic Bézier curve.
                            Vector2 v0 = p0 - Vector2(subpixelX, subpixelY);
                            Vector2 v1 = control - p0;
                            Vector2 v2 = p1 - control;

                            Real a = dot(v1, v1);
                            Real b = dot(v1, v2);
                            Real c = dot(v2, v2);
                            Real d = dot(v0, v1);
                            Real e = dot(v0, v2);

                            Real det = a * c - b * b;

                            // Parameters of the closest point on the curve to the point
                            Real s = b * e - c * d;
                            Real t = b * d - a * e;

                            Real distance;

                            if (s >= 0 && t >= 0 && s + t <= det) {
                                // The closest point is inside the triangle formed by p0, control, and p1
                                Real invDet = 1 / det;
                                s *= invDet;
                                t *= invDet;

                                // Calculate the distance to the curve
                                Vector2 closest_point = p0 + s * v1 + t * v2;
                                distance = length(Vector2(subpixelX, subpixelY) - closest_point);
                            } else {
                                // The closest point is outside the triangle, so find the distance to the closest endpoint
                                Real d0 = length(Vector2(subpixelX, subpixelY) - p0);
                                Real d1 = length(Vector2(subpixelX, subpixelY) - p1);
                                distance = std::min(d0, d1);
                            }
                            
                            // Check if the distance is within the curve's width.
                            if (distance <= quadratic->width * 0.5) {
                                Real alpha = quadratic->alpha;
                                Vector3 objectColor = quadratic->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }
                        } else if (auto *cubic = std::get_if<Cubic>(&shape)) {

                            Vector2 p0 = cubic->start;
                            Vector2 control1 = cubic->control1;
                            Vector2 control2 = cubic->control2;
                            Vector2 p1 = cubic->end;

                            // Calculate the distance from the object space center to the cubic Bézier curve.
                            const int iterations = 100;  // Number of iterations for the approximation
                            Real t = 0.0;
                            Real minDistance = length(p0 - Vector2(subpixelX, subpixelY));

                            // Approximate the minimum distance using a numerical method
                            for (int i = 0; i <= iterations; ++i) {
                                Real tCandidate = static_cast<Real>(i) / static_cast<Real>(iterations);
                                Real u = 1.0 - t;
                                Real tt = t * t;
                                Real uu = u * u;
                                Real uuu = uu * u;
                                Real ttt = tt * t;

                                Vector2 p = uuu * p0;  // (1-t)^3 * p0
                                p.x = p.x + 3 * uu * t * control1.x;  // 3 * (1-t)^2 * t * control1
                                p.y = p.y + 3 * uu * t * control1.y;  // 3 * (1-t) * t^2 * control2
                                
                                p.x = p.x + 3 * u * tt * control2.x;  // 3 * (1-t) * t^2 * control2
                                p.y = p.y + 3 * u * tt * control2.y;  // 3 * (1-t) * t^2 * control2
                                
                                p.x = p.x + ttt * p1.x;  // t^3 * p1
                                p.y = p.y + ttt * p1.y;  // t^3 * p1
                                
                                Vector2 curvePoint = p;
                                Real currentDistance = length(curvePoint - Vector2(subpixelX, subpixelY));

                                if (currentDistance < minDistance) {
                                    minDistance = currentDistance;
                                    t = tCandidate;
                                }
                            }

                            // Check if the distance is within the curve's width.
                            if (minDistance <= cubic->width * 0.5) {
                                Real alpha = cubic->alpha;
                                Vector3 objectColor = cubic->color;
                                subpixelColor = alpha * objectColor + (1 - alpha) * subpixelColor;
                                accum_alpha += alpha;
                            }
                        }
                    }
                    pixelColor += subpixelColor;
                }
            }
            pixelColor /= Real(16);
            img(x, y) = pixelColor;
        }
    }
    return img;
}
