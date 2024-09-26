#include "gameplay/gameplay_systems/TransformSystem.h"

const glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };
const glm::vec3 WorldRight = { 1.0f, 0.0f, 0.0f };
const glm::vec3 WorldForward = { 0.0f, 0.0f, 1.0f };

namespace oasis
{
	namespace gameplay
	{
		glm::vec3 Transform::Forward() const
		{
			glm::vec3 direction{ 1.0f };
			const glm::vec3& angles = rotation;
			direction.x = cos(glm::radians(angles.y)) * cos(glm::radians(angles.x));
			direction.y = sin(glm::radians(angles.x));
			direction.z = sin(glm::radians(angles.y)) * cos(glm::radians(angles.x));
			return direction;
		}

		glm::vec3 Transform::Up() const
		{ 
			return glm::cross(Right(), Forward()); 
		}

		glm::vec3 Transform::Right() const
		{ 
			return glm::normalize(glm::cross(Forward(), WorldUp)); 
		}

		glm::mat4 Transform::Model() const
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			model = glm::translate(model, position);

			model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, scale);

			return model;
		}

		Transform& TransformComponent::Transform()
		{
			return m_Transform;
		}

		void TransformComponent::Translate(const glm::vec3& translation)
		{
			m_Transform.position += translation;
		}

		void TransformComponent::Rotate(const glm::vec3& degrees)
		{
			m_Transform.rotation += degrees;
		}

		void TransformComponent::Scale(const glm::vec3& scaling)
		{
			m_Transform.scale += scaling;
		}

		void TransformSystem::Update(float deltaTime)
		{ }
	}
}