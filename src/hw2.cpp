#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single 3D triangle
    
    int img_width = 640;
    int img_height = 480;

    Image3 img(img_width, img_height);

    Vector3 p0{0, 0, -1};
    Vector3 p1{1, 0, -1};
    Vector3 p2{0, 1, -1};
    Real s = 1; // scaling factor of the view frustrum
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    Real z_near = 1e-6; // distance of the near clipping plane
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-p0") {
            p0.x = std::stof(params[++i]);
            p0.y = std::stof(params[++i]);
            p0.z = std::stof(params[++i]);
        } else if (params[i] == "-p1") {
            p1.x = std::stof(params[++i]);
            p1.y = std::stof(params[++i]);
            p1.z = std::stof(params[++i]);
        } else if (params[i] == "-p2") {
            p2.x = std::stof(params[++i]);
            p2.y = std::stof(params[++i]);
            p2.z = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        }
    }

    Vector2 h0 = Vector2(p0.x / -p0.z, p0.y / -p0.z);
    Vector2 h1 = Vector2(p1.x / -p1.z, p1.y / -p1.z);
    Vector2 h2 = Vector2(p2.x / -p2.z, p2.y / -p2.z);

    Real a = Real(img.width)/Real(img.height);

    Vector2 t0 = Vector2((img.width) * ((h0.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h0.y + (s)) / (2 * s)));
    Vector2 t1 = Vector2((img.width) * ((h1.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h1.y + (s)) / (2 * s)));
    Vector2 t2 = Vector2((img.width) * ((h2.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h2.y + (s)) / (2 * s)));

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 pixelColor = color;
            Vector3 subpixelColor = color;
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Vector2 subpixelCenter(subpixelX, subpixelY);

                    if(p0.z < -z_near && p1.z < -z_near && p2.z < -z_near) {
                        Vector2 e01 = t1 - t0;
                        Vector2 e12 = t2 - t1;
                        Vector2 e20 = t0 - t2;

                        Vector2 n01(e01.y, -e01.x);
                        Vector2 n12(e12.y, -e12.x);
                        Vector2 n20(e20.y, -e20.x);

                        Vector2 v0 = t0 - subpixelCenter;
                        Vector2 v1 = t1 - subpixelCenter;
                        Vector2 v2 = t2 - subpixelCenter;

                        if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                            subpixelColor = color;
                        } else {
                            subpixelColor = Vector3{0.5, 0.5, 0.5};
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

Image3 hw_2_2(const std::vector<std::string> &params) {
    // Homework 2.2: render a triangle mesh

    Image3 img(640 /* width */, 480 /* height */);
    Vector3 background = Vector3{0.5, 0.5, 0.5};

    Real s = 1; // scaling factor of the view frustrum
    Real a = Real(img.width)/Real(img.height);
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;

    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];

    Vector3 pixelColor = background;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 subpixelColor = background;
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {

                    Real z_min = INFINITY;

                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Vector2 subpixelCenter(subpixelX, subpixelY);

                    img.width = Real(img.width);
                    img.height = Real(img.height);

                    for (int l = 0; l < mesh.faces.size(); l++) {

                        Vector3 p0 = mesh.vertices[mesh.faces[l].x];
                        Vector3 p1 = mesh.vertices[mesh.faces[l].y];
                        Vector3 p2 = mesh.vertices[mesh.faces[l].z];

                        
                        if(-p0.z > z_near && -p1.z > z_near && -p2.z > z_near) {

                            Vector2 h0 = Vector2(p0.x / -p0.z, p0.y / -p0.z);
                            Vector2 h1 = Vector2(p1.x / -p1.z, p1.y / -p1.z);
                            Vector2 h2 = Vector2(p2.x / -p2.z, p2.y / -p2.z);

                            Vector2 t0 = Vector2((img.width) * ((h0.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h0.y + (s)) / (2 * s)));
                            Vector2 t1 = Vector2((img.width) * ((h1.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h1.y + (s)) / (2 * s)));
                            Vector2 t2 = Vector2((img.width) * ((h2.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h2.y + (s)) / (2 * s)));


                        
                            Vector2 e01 = t1 - t0;
                            Vector2 e12 = t2 - t1;
                            Vector2 e20 = t0 - t2;

                            Vector2 n01(e01.y, -e01.x);
                            Vector2 n12(e12.y, -e12.x);
                            Vector2 n20(e20.y, -e20.x);

                            Vector2 v0 = t0 - subpixelCenter;
                            Vector2 v1 = t1 - subpixelCenter;
                            Vector2 v2 = t2 - subpixelCenter;
                            
                            if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                    
                                Vector3 p_prime = Vector3(subpixelCenter.x, subpixelCenter.y, Real(1));
                                Vector3 p0_prime = Vector3(t0.x, t0.y, Real(1));
                                Vector3 p1_prime = Vector3(t1.x, t1.y, Real(1));
                                Vector3 p2_prime = Vector3(t2.x, t2.y, Real(1));

                                // Finding edge vectors
                                Vector3 e1 = p1_prime - p0_prime;
                                Vector3 e2 = p2_prime - p0_prime;

                                Real denominator = length(cross(e1, e2)) / Real(2);


                                Vector3 e3 = p1_prime - p_prime;
                                Vector3 e4 = p2_prime - p_prime;

                                Real b0_num = length(cross(e3, e4)) / Real(2);


                                Vector3 e5 = p_prime - p0_prime;
                                Vector3 e6 = p2_prime - p0_prime;

                                Real b1_num = length(cross(e5, e6)) / Real(2);


                                Vector3 e7 = p1_prime - p0_prime;
                                Vector3 e8 = p_prime - p0_prime;
                                
                                Real b2_num = length(cross(e7, e8)) / Real(2);


                                // getting barycentric prime coordinates
                                Real b0_prime = b0_num / denominator;
                                Real b1_prime = b1_num / denominator;
                                Real b2_prime = b2_num / denominator;

                                // getting barycentric coordinates
                                Real b_den = (b0_prime / p0.z) + (b1_prime / p1.z) + (b2_prime / p2.z);

                                Real b0 = (b0_prime / p0.z) / b_den;
                                Real b1 = (b1_prime / p1.z) / b_den;
                                Real b2 = (b2_prime / p2.z) / b_den;

                                // getting depth
                                Real depth = (b0 * p0.z + b1 * p1.z + b2 * p2.z);

                                // std::cout << depth << std::endl;
                                if (x == 306 && y == 184 && j == 0 && k == 0){
                                    std::cout << "Depth: "<< depth << ", " << "z_min: "<< z_min << "face color: "<< mesh.face_colors[l] << "face: "<< mesh.faces[l]<< std::endl;
                                }

                                depth = fabs(depth);

                                if (depth < z_min) {
                                    subpixelColor = mesh.face_colors[l];
                                    z_min = depth;
                                }
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

Image3 hw_2_3(const std::vector<std::string> &params) {
    // Homework 2.3: render a triangle mesh with vertex colors

    Image3 img(640 /* width */, 480 /* height */);
    Vector3 background = Vector3{0.5, 0.5, 0.5};

    Real s = 1; // scaling factor of the view frustrum
    Real a = Real(img.width)/Real(img.height);
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;

    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];

    Vector3 pixelColor = background;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 subpixelColor = background;
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {

                    Real z_min = INFINITY;

                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Vector2 subpixelCenter(subpixelX, subpixelY);

                    img.width = Real(img.width);
                    img.height = Real(img.height);

                    for (int l = 0; l < mesh.faces.size(); l++) {

                        Vector3 p0 = mesh.vertices[mesh.faces[l].x];
                        Vector3 p1 = mesh.vertices[mesh.faces[l].y];
                        Vector3 p2 = mesh.vertices[mesh.faces[l].z];

                        
                        if(-p0.z > z_near && -p1.z > z_near && -p2.z > z_near) {

                            Vector2 h0 = Vector2(p0.x / -p0.z, p0.y / -p0.z);
                            Vector2 h1 = Vector2(p1.x / -p1.z, p1.y / -p1.z);
                            Vector2 h2 = Vector2(p2.x / -p2.z, p2.y / -p2.z);

                            Vector2 t0 = Vector2((img.width) * ((h0.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h0.y + (s)) / (2 * s)));
                            Vector2 t1 = Vector2((img.width) * ((h1.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h1.y + (s)) / (2 * s)));
                            Vector2 t2 = Vector2((img.width) * ((h2.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h2.y + (s)) / (2 * s)));


                        
                            Vector2 e01 = t1 - t0;
                            Vector2 e12 = t2 - t1;
                            Vector2 e20 = t0 - t2;

                            Vector2 n01(e01.y, -e01.x);
                            Vector2 n12(e12.y, -e12.x);
                            Vector2 n20(e20.y, -e20.x);

                            Vector2 v0 = t0 - subpixelCenter;
                            Vector2 v1 = t1 - subpixelCenter;
                            Vector2 v2 = t2 - subpixelCenter;
                            
                            if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                    
                                Vector3 p_prime = Vector3(subpixelCenter.x, subpixelCenter.y, Real(1));
                                Vector3 p0_prime = Vector3(t0.x, t0.y, Real(1));
                                Vector3 p1_prime = Vector3(t1.x, t1.y, Real(1));
                                Vector3 p2_prime = Vector3(t2.x, t2.y, Real(1));

                                // Finding edge vectors
                                Vector3 e1 = p1_prime - p0_prime;
                                Vector3 e2 = p2_prime - p0_prime;

                                Real denominator = length(cross(e1, e2)) / Real(2);


                                Vector3 e3 = p1_prime - p_prime;
                                Vector3 e4 = p2_prime - p_prime;

                                Real b0_num = length(cross(e3, e4)) / Real(2);


                                Vector3 e5 = p_prime - p0_prime;
                                Vector3 e6 = p2_prime - p0_prime;

                                Real b1_num = length(cross(e5, e6)) / Real(2);


                                Vector3 e7 = p1_prime - p0_prime;
                                Vector3 e8 = p_prime - p0_prime;
                                
                                Real b2_num = length(cross(e7, e8)) / Real(2);


                                // getting barycentric prime coordinates
                                Real b0_prime = b0_num / denominator;
                                Real b1_prime = b1_num / denominator;
                                Real b2_prime = b2_num / denominator;

                                // getting barycentric coordinates
                                Real b_den = (b0_prime / p0.z) + (b1_prime / p1.z) + (b2_prime / p2.z);

                                Real b0 = (b0_prime / p0.z) / b_den;
                                Real b1 = (b1_prime / p1.z) / b_den;
                                Real b2 = (b2_prime / p2.z) / b_den;

                                // getting depth
                                Real depth = (b0 * p0.z + b1 * p1.z + b2 * p2.z);

                                Vector3 C0 = mesh.vertex_colors[mesh.faces[l].x];
                                Vector3 C1 = mesh.vertex_colors[mesh.faces[l].y];
                                Vector3 C2 = mesh.vertex_colors[mesh.faces[l].z];

                                Vector3 interpolatedColor = b0 * C0 + b1 * C1 + b2 * C2;

                                // std::cout << depth << std::endl;
                                if (x == 306 && y == 184 && j == 0 && k == 0){
                                    std::cout << "Depth: "<< depth << ", " << "z_min: "<< z_min << "face color: "<< mesh.face_colors[l] << "face: "<< mesh.faces[l]<< std::endl;
                                }

                                depth = fabs(depth);

                                if (depth < z_min) {
                                    subpixelColor = interpolatedColor;
                                    z_min = depth;
                                }
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

Image3 hw_2_4(const std::vector<std::string> &params) {
    // Homework 2.4: render a scene with transformation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
    

    Image3 img(scene.camera.resolution.x,
               scene.camera.resolution.y);

    Vector3 background = Vector3{0.5, 0.5, 0.5};

    Real s = 1; // scaling factor of the view frustrum
    Real a = Real(img.width)/Real(img.height);
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;

    Vector3 pixelColor = background;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 subpixelColor = background;
        
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {

                    Real z_min = INFINITY;

                    Real subpixelX = (x + (j + 0.5) / 4.0);
                    Real subpixelY = (y + (k + 0.5) / 4.0);
                    Vector2 subpixelCenter(subpixelX, subpixelY);

                    img.width = Real(img.width);
                    img.height = Real(img.height);

                    for (auto& mesh : scene.meshes) {
                        for (int l = 0; l < mesh.faces.size(); l++) {

                            Vector3 p0 = mesh.vertices[mesh.faces[l].x];
                            Vector3 p1 = mesh.vertices[mesh.faces[l].y];
                            Vector3 p2 = mesh.vertices[mesh.faces[l].z];

                            
                            if(-p0.z > z_near && -p1.z > z_near && -p2.z > z_near) {

                                Vector2 h0 = Vector2(p0.x / -p0.z, p0.y / -p0.z);
                                Vector2 h1 = Vector2(p1.x / -p1.z, p1.y / -p1.z);
                                Vector2 h2 = Vector2(p2.x / -p2.z, p2.y / -p2.z);

                                Vector2 t0 = Vector2((img.width) * ((h0.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h0.y + (s)) / (2 * s)));
                                Vector2 t1 = Vector2((img.width) * ((h1.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h1.y + (s)) / (2 * s)));
                                Vector2 t2 = Vector2((img.width) * ((h2.x + (s * a)) / (2 * s * a)), (img.height) * ((Real(-1) * h2.y + (s)) / (2 * s)));


                            
                                Vector2 e01 = t1 - t0;
                                Vector2 e12 = t2 - t1;
                                Vector2 e20 = t0 - t2;

                                Vector2 n01(e01.y, -e01.x);
                                Vector2 n12(e12.y, -e12.x);
                                Vector2 n20(e20.y, -e20.x);

                                Vector2 v0 = t0 - subpixelCenter;
                                Vector2 v1 = t1 - subpixelCenter;
                                Vector2 v2 = t2 - subpixelCenter;
                                
                                if ((dot(v0, n01) >= 0) && (dot(v1, n12) >= 0) && (dot(v2, n20) >= 0) || (dot(v0, n01) <= 0) && (dot(v1, n12) <= 0) && (dot(v2, n20) <= 0)) {
                                        
                                    Vector3 p_prime = Vector3(subpixelCenter.x, subpixelCenter.y, Real(1));
                                    Vector3 p0_prime = Vector3(t0.x, t0.y, Real(1));
                                    Vector3 p1_prime = Vector3(t1.x, t1.y, Real(1));
                                    Vector3 p2_prime = Vector3(t2.x, t2.y, Real(1));

                                    // Finding edge vectors
                                    Vector3 e1 = p1_prime - p0_prime;
                                    Vector3 e2 = p2_prime - p0_prime;

                                    Real denominator = length(cross(e1, e2)) / Real(2);


                                    Vector3 e3 = p1_prime - p_prime;
                                    Vector3 e4 = p2_prime - p_prime;

                                    Real b0_num = length(cross(e3, e4)) / Real(2);


                                    Vector3 e5 = p_prime - p0_prime;
                                    Vector3 e6 = p2_prime - p0_prime;

                                    Real b1_num = length(cross(e5, e6)) / Real(2);


                                    Vector3 e7 = p1_prime - p0_prime;
                                    Vector3 e8 = p_prime - p0_prime;
                                    
                                    Real b2_num = length(cross(e7, e8)) / Real(2);


                                    // getting barycentric prime coordinates
                                    Real b0_prime = b0_num / denominator;
                                    Real b1_prime = b1_num / denominator;
                                    Real b2_prime = b2_num / denominator;

                                    // getting barycentric coordinates
                                    Real b_den = (b0_prime / p0.z) + (b1_prime / p1.z) + (b2_prime / p2.z);

                                    Real b0 = (b0_prime / p0.z) / b_den;
                                    Real b1 = (b1_prime / p1.z) / b_den;
                                    Real b2 = (b2_prime / p2.z) / b_den;

                                    // getting depth
                                    Real depth = (b0 * p0.z + b1 * p1.z + b2 * p2.z);

                                    Vector3 C0 = mesh.vertex_colors[mesh.faces[l].x];
                                    Vector3 C1 = mesh.vertex_colors[mesh.faces[l].y];
                                    Vector3 C2 = mesh.vertex_colors[mesh.faces[l].z];

                                    Vector3 interpolatedColor = b0 * C0 + b1 * C1 + b2 * C2;

                                    depth = fabs(depth);

                                    if (depth < z_min) {
                                        subpixelColor = interpolatedColor;
                                        z_min = depth;
                                    }
                                }
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