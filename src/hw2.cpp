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

Vector3 xform_point(const Matrix4x4 &m, const Vector3 &p) {
    Vector4 ph = Vector4{p.x, p.y, p.z, Real(1)};
    Vector4 mph = m * ph;
    return Vector3{mph.x / mph.w,
                   mph.y / mph.w,
                   mph.z / mph.w};
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
    // Construct perspective projection matrix
    Real w = scene.camera.resolution.x;
    Real h = scene.camera.resolution.y;
    Real s = scene.camera.s;
    Real a = Real(w) / Real(h);

    Matrix4x4 projection;
    projection(0, 0) = 1;
    projection(1, 1) = 1;
    projection(2, 3) = 1;
    projection(3, 2) = -1;
    Matrix4x4 camera_to_screen;
    camera_to_screen(0, 0) = w / (2 * s * a);
    camera_to_screen(0, 3) = w / 2;
    camera_to_screen(1, 1) = -h / (2 * s);
    camera_to_screen(1, 3) = h / 2;
    camera_to_screen(2, 2) = 1;
    camera_to_screen(3, 3) = 1;

    Matrix4x4 world_to_screen =
        camera_to_screen * projection * inverse(scene.camera.cam_to_world);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 sum{0, 0, 0};
            int n = 4;
            for (int dx = 0; dx < n; dx++) {
                for (int dy = 0; dy < n; dy++) {
                    Real xoff = (dx + Real(0.5)) / Real(n);
                    Real yoff = (dy + Real(0.5)) / Real(n);
                    Vector2 pixel_center{x + xoff, y + yoff};
                    Vector3 color = Vector3{0.5, 0.5, 0.5}; // background color
                    Real z_min = infinity<Real>();
                    for (const auto &mesh : scene.meshes) {
                        // construct the MVP matrix
                        Matrix4x4 MVP = world_to_screen * mesh.model_matrix;
                        for (int i = 0; i < (int)mesh.faces.size(); i++) {
                            Vector3 p0 = mesh.vertices[mesh.faces[i][0]];
                            Vector3 p1 = mesh.vertices[mesh.faces[i][1]];
                            Vector3 p2 = mesh.vertices[mesh.faces[i][2]];
                            Vector3 screen_p0 = xform_point(MVP, p0);
                            Vector3 screen_p1 = xform_point(MVP, p1);
                            Vector3 screen_p2 = xform_point(MVP, p2);

                            // We don't implement clipping here, if even one of the 
                            // vertex is behind the clipping plane, we skip the triangle
                            // screen_pi.z stores 1/pi.z
                            Real inv_z_near = (1/scene.camera.z_near);
                            if (screen_p0.z > inv_z_near ||
                                    screen_p1.z > inv_z_near ||
                                    screen_p2.z > inv_z_near) {
                                continue;
                            }

                            Vector2 sp0 = Vector2{screen_p0.x, screen_p0.y};
                            Vector2 sp1 = Vector2{screen_p1.x, screen_p1.y};
                            Vector2 sp2 = Vector2{screen_p2.x, screen_p2.y};

                            Vector2 e01 = sp1 - sp0, e12 = sp2 - sp1, e20 = sp0 - sp2;
                            auto normal = [](const Vector2 &v) {
                                return Vector2{v.y, -v.x};
                            };
                            Vector2 n01 = normal(e01),
                                    n12 = normal(e12),
                                    n20 = normal(e20);

                            bool s01 = dot(pixel_center - sp0, n01) > 0;
                            bool s12 = dot(pixel_center - sp1, n12) > 0;
                            bool s20 = dot(pixel_center - sp2, n20) > 0;
                            if ((s01 && s12 && s20) || (!s01 && !s12 && !s20)) {
                                // Inside the triangle
                                // Interpolate Z
                                
                                // 1. convert pixel center from screen space to camera space
                                auto to_camera = [&](const Vector2 &p) {
                                    // camera to screen:
                                    // x'' = w ((x' + sa)/ (2sa))
                                    // y'' = -h ((y' - s)/ (2s))
                                    // screen to camera:
                                    // x' = 2sa x'' / w - sa
                                    // y' = - 2s y'' / h + s
                                    return Vector2{(2 * s * a) * p.x / img.width - s * a,
                                                   - 2 * s * p.y / img.height + s};
                                };
                                Vector2 pp = to_camera(pixel_center);
                                Vector2 pp0 = to_camera(sp0);
                                Vector2 pp1 = to_camera(sp1);
                                Vector2 pp2 = to_camera(sp2);

                                // 2. compute projected barycentric coordinates
                                auto area = [&](const Vector2 pp0,
                                                const Vector2 pp1,
                                                const Vector2 pp2) {
                                    // lift things to 3D to take cross product
                                    Vector3 p0 = Vector3{pp0.x, pp0.y, Real(0)};
                                    Vector3 p1 = Vector3{pp1.x, pp1.y, Real(0)};
                                    Vector3 p2 = Vector3{pp2.x, pp2.y, Real(0)};
                                    return length(cross(p1 - p0, p2 - p0)) / 2;
                                };

                                Real a0 = area(pp, pp1, pp2);
                                Real a1 = area(pp0, pp, pp2);
                                Real a2 = area(pp0, pp1, pp);
                                Real denom = area(pp0, pp1, pp2);
                                Real b0p = a0 / denom;
                                Real b1p = a1 / denom;
                                Real b2p = a2 / denom;

                                // 3. compute the original barycentric coordinates
                                b0p *= screen_p0.z;
                                b1p *= screen_p1.z;
                                b2p *= screen_p2.z;
                                Real b0 = b0p / (b0p + b1p + b2p);
                                Real b1 = b1p / (b0p + b1p + b2p);
                                Real b2 = b2p / (b0p + b1p + b2p);

                                // 4. interpolate and obtain Z
                                Real z = b0 / screen_p0.z +
                                         b1 / screen_p1.z +
                                         b2 / screen_p2.z;
                                // Remember to negate to make Z positive
                                z = fabs(z);

                                // Now, test if Z is smaller
                                if (z < z_min) {
                                    // Interpolate vertex colors
                                    Vector3 C0 = mesh.vertex_colors[mesh.faces[i][0]];
                                    Vector3 C1 = mesh.vertex_colors[mesh.faces[i][1]];
                                    Vector3 C2 = mesh.vertex_colors[mesh.faces[i][2]];

                                    color = b0 * C0 + b1 * C1 + b2 * C2;
                                    z_min = z;
                                }
                            }
                        }
                    }
                    sum += color;
                }
            }
            img(x, y) = sum / Real(n * n);
        }
    }
    return img;
}
