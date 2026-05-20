/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** Renderer.hpp - Graphics/Ray computation logic
*/

#pragma once

#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include <algorithm>

#include "IObject.hpp"
#include "ILight.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Tile.hpp"

namespace raytracer::threading {

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    /**
     * @brief Compute ray-tracing for a single pixel
     * @param objects List of scene objects
     * @param lights List of scene lights
     * @param camera Camera parameters (position, resolution, FOV)
     * @param tile Output tile to write color to
     * @param x Pixel X coordinate
     * @param y Pixel Y coordinate
     * @param width Image width
     */
    void ComputePixel(
        const std::vector<std::unique_ptr<IObject>>& objects,
        const std::vector<std::unique_ptr<ILight>>& lights,
        const render::Camera& camera,
        Tile& tile,
        std::size_t x,
        std::size_t y,
        std::size_t width
    );

private:
    /**
     * @brief Generate sky color based on Y coordinate
     * @param y Pixel Y coordinate
     * @param height Image height
     * @return RGB color array
     */
    std::array<int, 3> GenerateSkyColor(std::size_t y, std::size_t height);

    /**
     * @brief Generate ray from camera for given pixel
     * @param camera Camera parameters
     * @param x Pixel X coordinate
     * @param y Pixel Y coordinate
     * @param width Image width
     * @return Generated ray
     */
    core::Ray GenerateRay(
        const render::Camera& camera,
        std::size_t x,
        std::size_t y,
        std::size_t width
    );

    /**
     * @brief Find closest object intersection with ray
     * @param objects List of scene objects
     * @param ray Ray to test
     * @param outDistance Output: distance to closest intersection
     * @return Pointer to closest object, or nullptr if no intersection
     */
    const IObject* FindClosestIntersection(
        const std::vector<std::unique_ptr<IObject>>& objects,
        const core::Ray& ray,
        double& outDistance
    );

    /**
     * @brief Check if a point is in shadow for a given light direction
     * @param objects List of scene objects
     * @param shadowOrigin Origin of the shadow ray (hit point + offset)
     * @param lightDir Normalized direction toward the light
     * @param maxDistance Max distance to check (use 1e30 for directional, distToLight for point)
     * @return true if the point is in shadow
     */
    bool IsInShadow(
        const std::vector<std::unique_ptr<IObject>>& objects,
        const math::Point3D& shadowOrigin,
        const math::Vector3D& lightDir,
        double maxDistance
    );

    /**
     * @brief Compute shading for hit point using scene lights
     * @param objects List of scene objects (for shadow rays)
     * @param hitObject Object that was hit
     * @param hitPoint 3D point of intersection
     * @param ray Ray that hit
     * @param lights List of scene lights
     * @return RGB shaded color
     */
    std::array<int, 3> ComputeShading(
        const std::vector<std::unique_ptr<IObject>>& objects,
        const IObject* hitObject,
        const math::Point3D& hitPoint,
        const core::Ray& ray,
        const std::vector<std::unique_ptr<ILight>>& lights
    );
};

} // namespace raytracer::threading