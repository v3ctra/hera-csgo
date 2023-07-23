#pragma once
#include <numbers>
#include <limits>
#include <cmath>

//w t f
template <typename T> constexpr auto deg2rad(T degrees) noexcept { return degrees * (std::numbers::pi_v<T> / static_cast<T>(180)); }
template <typename T> constexpr auto rad2deg(T radians) noexcept { return radians * (static_cast<T>(180) / std::numbers::pi_v<T>); }

namespace math {
	struct vec3_t {
		float x, y, z;

		constexpr vec3_t(float x = 0.f, float y = 0.f, float z = 0.f) :
			x(x), y(y), z(z) { }

		constexpr vec3_t(const float* arrVector) :
			x(arrVector[0]), y(arrVector[1]), z(arrVector[2]) { }

		[[nodiscard]] bool is_valid() const {
			return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
		}

		constexpr void invalidate() {
			this->x = this->y = this->z = std::numeric_limits<float>::infinity();
		}

		[[nodiscard]] float* data() {
			return reinterpret_cast<float*>(this);
		}

		[[nodiscard]] const float* data() const {
			return reinterpret_cast<float*>(const_cast<vec3_t*>(this));
		}

		float& operator[](const std::size_t i) {
			return this->data()[i];
		}

		const float& operator[ ](const std::size_t i) const {
			return this->data()[i];
		}

		bool operator==(const vec3_t& vec_base) const {
			return this->is_equal(vec_base);
		}

		bool operator!=(const vec3_t& vec_base) const {
			return !this->is_equal(vec_base);
		}

		constexpr vec3_t& operator+=(const vec3_t& vec_base) {
			this->x += vec_base.x; this->y += vec_base.y; this->z += vec_base.z;
			return *this;
		}

		constexpr vec3_t& operator-=(const vec3_t& vec_base) {
			this->x -= vec_base.x; this->y -= vec_base.y; this->z -= vec_base.z;
			return *this;
		}

		constexpr vec3_t& operator*=(const vec3_t& vec_base) {
			this->x *= vec_base.x; this->y *= vec_base.y; this->z *= vec_base.z;
			return *this;
		}

		constexpr vec3_t& operator/=(const vec3_t& vec_base) {
			this->x /= vec_base.x; this->y /= vec_base.y; this->z /= vec_base.z;
			return *this;
		}

		constexpr vec3_t& operator+=(const float add) {
			this->x += add; this->y += add; this->z += add;
			return *this;
		}

		constexpr vec3_t& operator-=(const float subtract) {
			this->x -= subtract; this->y -= subtract; this->z -= subtract;
			return *this;
		}

		constexpr vec3_t& operator*=(const float multiply) {
			this->x *= multiply; this->y *= multiply; this->z *= multiply;
			return *this;
		}

		constexpr vec3_t& operator/=(const float divide) {
			this->x /= divide; this->y /= divide; this->z /= divide;
			return *this;
		}

		vec3_t operator+(const vec3_t& vec_add) const {
			return vec3_t(this->x + vec_add.x, this->y + vec_add.y, this->z + vec_add.z);
		}

		vec3_t operator-(const vec3_t& vec_subtract) const {
			return vec3_t(this->x - vec_subtract.x, this->y - vec_subtract.y, this->z - vec_subtract.z);
		}

		vec3_t operator*(const vec3_t& vec_multiply) const {
			return vec3_t(this->x * vec_multiply.x, this->y * vec_multiply.y, this->z * vec_multiply.z);
		}

		vec3_t operator/(const vec3_t& vec_divide) const {
			return vec3_t(this->x / vec_divide.x, this->y / vec_divide.y, this->z / vec_divide.z);
		}

		vec3_t operator+(const float add) const {
			return vec3_t(this->x + add, this->y + add, this->z + add);
		}

		vec3_t operator-(const float subtract) const {
			return vec3_t(this->x - subtract, this->y - subtract, this->z - subtract);
		}

		vec3_t operator*(const float multiply) const {
			return vec3_t(this->x * multiply, this->y * multiply, this->z * multiply);
		}

		vec3_t operator/(const float divide) const {
			return vec3_t(this->x / divide, this->y / divide, this->z / divide);
		}

		[[nodiscard]] bool is_equal(const vec3_t& vec_equal, const float error_margin = std::numeric_limits<float>::epsilon()) const {
			return (std::fabsf(this->x - vec_equal.x) < error_margin &&
				std::fabsf(this->y - vec_equal.y) < error_margin &&
				std::fabsf(this->z - vec_equal.z) < error_margin);
		}

		[[nodiscard]] bool is_zero() const {
			return (std::fpclassify(this->x) == FP_ZERO &&
				std::fpclassify(this->y) == FP_ZERO &&
				std::fpclassify(this->z) == FP_ZERO);
		}

		[[nodiscard]] float length() const {
			return std::sqrtf(this->length_sqr());
		}

		[[nodiscard]] constexpr float length_sqr() const {
			return dot_product(*this);
		}

		[[nodiscard]] float length_2d() const {
			return std::sqrtf(this->length2d_sqr());
		}

		[[nodiscard]] constexpr float length2d_sqr() const {
			return (this->x * this->x + this->y * this->y);
		}

		[[nodiscard]] float dist_to(const vec3_t& vec_end) const {
			return (*this - vec_end).length();
		}

		inline float dist_2d(const vec3_t& vec) {
			vec3_t delta;

			delta.x = x - vec.x;
			delta.y = y - vec.y;
			delta.z = z - vec.z;

			return delta.length_2d();
		}

		[[nodiscard]] constexpr float dist_tosqr(const vec3_t& vec_end) const {
			return (*this - vec_end).length_sqr();
		}

		[[nodiscard]] void clamp() {
			x = std::clamp(x, -89.0f, 89.0f);
			y = std::clamp(y, -180.f, 180.f);
			z = std::clamp(z, -50.0f, 50.0f);
		}
		vec3_t normalize() {
			this->x = std::isfinite(this->x) ? std::remainderf(this->x, 360.f) : 0.f;
			this->y = std::isfinite(this->y) ? std::remainderf(this->y, 360.f) : 0.f;
			this->z = std::clamp(this->z, -50.f, 50.f);
			return *this;
		}

		[[nodiscard]] vec3_t normalized() const {
			vec3_t vecOut = *this;
			vecOut.normalize_in_place();
			return vecOut;
		}

		float normalize_in_place() {
			const float flLength = this->length();
			const float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

			this->x *= flRadius;
			this->y *= flRadius;
			this->z *= flRadius;

			return flLength;
		}

		auto to_angle() const noexcept
		{
			return vec3_t{ rad2deg(std::atan2(-z, std::hypot(x, y))),
							rad2deg(std::atan2(y, x)),
							0.0f };
		}

		static auto fromAngle(const vec3_t& angle) noexcept
		{
			return vec3_t{ std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y)),
							std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y)),
							-std::sin(deg2rad(angle.x)) };
		}

		[[nodiscard]] constexpr float dot_product(const vec3_t& vec_dot) const {
			return (this->x * vec_dot.x + this->y * vec_dot.y + this->z * vec_dot.z);
		}

		[[nodiscard]] constexpr vec3_t cross_product(const vec3_t& vec_cross) const {
			return vec3_t(this->y * vec_cross.z - this->z * vec_cross.y, this->z * vec_cross.x - this->x * vec_cross.z, this->x * vec_cross.y - this->y * vec_cross.x);
		}
	};

	struct matrix3x4_t {
		float* operator[ ](int index) {
			return matrix[index];
		}
		float matrix[3][4];
	};

	__forceinline vec3_t calculate_angle(const math::vec3_t source, const math::vec3_t destination, const math::vec3_t viewAngles) {
		return ((destination - source).to_angle() - viewAngles).normalize();
	}
}