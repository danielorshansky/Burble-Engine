#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Movement {
	Forward, Backward, Left, Right, Up, Down
};

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 2.f), glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), glm::vec3 front = glm::vec3(0.f, 0.f, -1.f), float yaw = -90.f, float pitch = 0.f, float zoom = 60.f, float speed = 2.5f, float sensitivity = 0.1f, float zoom_max = 135.f, float zoom_min = 45.f, float near = 0.1f, float far = 100.f, float pitch_max = 89.f, float pitch_min = -89.f) : world_up(world_up), position(position), yaw(yaw), pitch(pitch), zoom(zoom), speed(speed), sensitivity(sensitivity), zoom_max(zoom_max), zoom_min(zoom_min), front(front), near(near), far(far), pitch_max(pitch_max), pitch_min(pitch_min) {
		Update();
	}

	static Camera Create(float position_x = 0.f, float position_y = 0.f, float position_z = 2.f, float world_up_x = 0.f, float world_up_y = 1.f, float world_up_z = 0.f, float front_x = 0.f, float front_y = 0.f, float front_z = -1.f, float yaw = -90.f, float pitch = 0.f, float zoom = 45.f, float speed = 2.5f, float sensitivity = 0.1f, float zoom_max = 45.f, float zoom_min = 20.f, float near = 0.1f, float far = 100.f, float pitch_max = 89.f, float pitch_min = -89.f) {
		return Camera(glm::vec3(position_x, position_y, position_z), glm::vec3(world_up_x, world_up_y, world_up_z), glm::vec3(front_x, front_y, front_z), yaw, pitch, zoom, speed, sensitivity, zoom_max, zoom_min, near, far, pitch_max, pitch_min);
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, position + front, up); // generate view matrix
	}

	glm::mat4 GetProjectionMatrix(float aspect_ratio) {
		return glm::perspective(glm::radians(zoom), aspect_ratio, near, far); // give projection matrix given aspect ratio, near, and far
	}

	void Translate(Movement direction, float delta_time = 1.f) { // move camera
		float velocity = speed * delta_time;
		if (direction == Movement::Forward)
			position += front * velocity;
		if (direction == Movement::Backward)
			position -= front * velocity;
		if (direction == Movement::Left)
			position -= right * velocity;
		if (direction == Movement::Right)
			position += right * velocity;
		if (direction == Movement::Up)
			position += up * velocity;
		if (direction == Movement::Down)
			position -= up * velocity;
	}

	void Rotate(float x_offset, float y_offset, bool constrain_pitch = false) { // rotate camera
		x_offset *= sensitivity;
		y_offset *= sensitivity;
		yaw += x_offset;
		pitch += y_offset;
		if (pitch > pitch_max && constrain_pitch)
			pitch = pitch_max;
		if (pitch < pitch_min && constrain_pitch)
			pitch = pitch_min;
		Update();
	}

	float GetZoom() {
		return zoom;
	}

	void Zoom(float zoom_offset) { // zoom camera
		zoom -= zoom_offset;
		if (zoom < zoom_min)
			zoom = zoom_min;
		if (zoom > zoom_max)
			zoom = zoom_max;
	}

private:
	glm::vec3 position, front, up, world_up, right;
	float yaw, pitch, speed, sensitivity, zoom, zoom_max, zoom_min, near, far, pitch_min, pitch_max; // camera values

	void Update() { // update camera vectors
		glm::vec3 updated_front;
		updated_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		updated_front.y = sin(glm::radians(pitch));
		updated_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(updated_front);
		right = glm::normalize(glm::cross(front, world_up));
		up = glm::normalize(glm::cross(right, front));
	}
};
