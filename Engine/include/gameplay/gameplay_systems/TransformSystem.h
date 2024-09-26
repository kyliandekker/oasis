#pragma once

#include "gameplay/System.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace oasis
{
	namespace gameplay
	{
		enum class ETransfrom
		{
			position, rotation, scale
		};
		struct Transform
		{
			Transform() {};
			Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : position(pos), rotation(rot), scale(scale) {};

			glm::vec3 position{ 0.0f };
			glm::vec3 rotation{ 0.0f };
			glm::vec3 scale{ 1.0f };

			glm::vec3 Forward() const;
			glm::vec3 Up() const;
			glm::vec3 Right() const;
			glm::mat4 Model() const;
		};

		class TransformComponent
		{
		private:
			Transform m_Transform;
		public:
			TransformComponent() {};
			TransformComponent(const Transform& a_Transform) : m_Transform(a_Transform) {};
			Transform& Transform();

			void Translate(const glm::vec3& translation);

			void Rotate(const glm::vec3& degrees);

			void Scale(const glm::vec3& scaling);
		};

		class TransformSystem : public System<TransformComponent>
		{
		public:
			TransformSystem(unsigned int priority) : System(priority) {}
			~TransformSystem() = default;

			virtual void Update(float deltaTime) override;
		};
	}
}
