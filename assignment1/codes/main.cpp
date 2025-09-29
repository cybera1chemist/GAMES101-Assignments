#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float angle_x, float angle_y, float angle_z)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    // Asume that the rotation_angle is given in degrees
    float angle_x_rad = angle_x * MY_PI / 180.0;
    float angle_y_rad = angle_y * MY_PI / 180.0;
    float angle_z_rad = angle_z * MY_PI / 180.0;


    Eigen::Matrix4f rotation_x, rotation_y, rotation_z;
    rotation_x << 1,              0,               0, 0,
                  0, cos(angle_x_rad), -sin(angle_x_rad), 0,
                  0, sin(angle_x_rad),  cos(angle_x_rad), 0,
                  0,              0,               0, 1;
    rotation_y <<  cos(angle_y_rad), 0, sin(angle_y_rad), 0,
                          0, 1,              0, 0,
                -sin(angle_y_rad), 0, cos(angle_y_rad), 0,
                          0, 0,              0, 1;
    rotation_z << cos(angle_z_rad), -sin(angle_z_rad), 0, 0,
             sin(angle_z_rad),  cos(angle_z_rad), 0, 0,
                         0,              0, 1, 0,
                         0,              0, 0, 1;
    model = rotation_z * rotation_y * rotation_x;
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    float fov_rad = eye_fov * MY_PI / 180.0;
    float t = tan(fov_rad / 2) * fabs(zNear);
    projection << zNear / (aspect_ratio * t), 0, 0, 0,
                  0, zNear / t, 0, 0,
                  0, 0, (zNear + zFar) / (zNear - zFar), (2 * zNear * zFar) / (zFar - zNear),
                  0, 0, 1, 0;
    

    return projection;
}

int main(int argc, const char** argv)
{
    float angle_z = 0;
    float angle_x = 0;
    float angle_y = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle_z = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    // compute triangle centroid so we can rotate around the triangle's center
    Eigen::Vector3f centroid = (pos[0] + pos[1] + pos[2]) / 3.0f;

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // rotate around the triangle centroid: translate to origin -> rotate -> translate back
        {
            Eigen::Matrix4f rotation = get_model_matrix(angle_x, angle_y, angle_z);
            Eigen::Matrix4f translate_to_origin = Eigen::Matrix4f::Identity();
            Eigen::Matrix4f translate_back = Eigen::Matrix4f::Identity();
            translate_to_origin << 1, 0, 0, -centroid[0],
                                  0, 1, 0, -centroid[1],
                                  0, 0, 1, -centroid[2],
                                  0, 0, 0, 1;
            translate_back << 1, 0, 0, centroid[0],
                             0, 1, 0, centroid[1],
                             0, 0, 1, centroid[2],
                             0, 0, 0, 1;

            Eigen::Matrix4f model = translate_back * rotation * translate_to_origin;
            r.set_model(model);
        }
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // rotate around the triangle centroid: translate to origin -> rotate -> translate back
        {
            Eigen::Matrix4f rotation = get_model_matrix(angle_x, angle_y, angle_z);
            Eigen::Matrix4f translate_to_origin = Eigen::Matrix4f::Identity();
            Eigen::Matrix4f translate_back = Eigen::Matrix4f::Identity();
            translate_to_origin << 1, 0, 0, -centroid[0],
                                  0, 1, 0, -centroid[1],
                                  0, 0, 1, -centroid[2],
                                  0, 0, 0, 1;
            translate_back << 1, 0, 0, centroid[0],
                             0, 1, 0, centroid[1],
                             0, 0, 1, centroid[2],
                             0, 0, 0, 1;

            Eigen::Matrix4f model = translate_back * rotation * translate_to_origin;
            r.set_model(model);
        }
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'q') {
            angle_z += 10;
        }
        else if (key == 'e') {
            angle_z -= 10;
        }
        else if (key == 's') {
            angle_x -= 10;
        }
        else if (key == 'w') {
            angle_x += 10;
        }
        else if (key == 'a') {
            angle_y -= 10;
        }
        else if (key == 'd') {
            angle_y += 10;
        }
    }

    return 0;
}
