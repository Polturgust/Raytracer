/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** Renderer.cpp - Graphics/Ray computation implementation
*/

#include "Renderer.hpp"

namespace raytracer::threading {

std::array<int, 3> Renderer::GenerateSkyColor(std::size_t y, std::size_t height)
{
    const auto clampColor = [](double value) -> int {
        if (value < 0.0)
            return 0;
        if (value > 255.0)
            return 255;
        return static_cast<int>(value);
    };
    const double ratio = static_cast<double>(y) / static_cast<double>(height == 0 ? 1 : height);
    return {
        clampColor(20.0 + 80.0 * ratio),
        clampColor(40.0 + 120.0 * ratio),
        clampColor(70.0 + 150.0 * ratio)
    };
}

core::Ray Renderer::GenerateRay(
    const render::Camera& camera,
    std::size_t x,
    std::size_t y,
    std::size_t width)
{
    const double pi = 3.14159265358979323846;
    const double fovRad = camera.fieldOfView * (pi / 180.0);
    const double imageScale = std::tan(fovRad * 0.5);
    const double height = static_cast<double>(camera.resolution.second == 0 ? 1 : camera.resolution.second);
    const double dWidth = static_cast<double>(width);
    const double aspect = dWidth / height;

    const math::Point3D origin(
        static_cast<double>(camera.position[0]),
        static_cast<double>(camera.position[1]),
        static_cast<double>(camera.position[2]));

    const double dX = static_cast<double>(x);
    const double dY = static_cast<double>(y);
    const double sx = ((dX + 0.5) / dWidth * 2.0 - 1.0) * aspect * imageScale;
    const double sy = (1.0 - (dY + 0.5) / height * 2.0) * imageScale;

    const math::Vector3D direction(sx, sy, 1.0);

    return core::Ray(origin, direction.normalized());
}

const IObject* Renderer::FindClosestIntersection(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const core::Ray& ray,
    double& outDistance)
{
    const IObject* hitObject = nullptr;
    double closestDistance = 1e30;

    for (const auto& object : objects) {
        if (!object)
            continue;
        double distance = 0.0;
        if (object->Intersect(ray, distance) && distance > 0.001 && distance < closestDistance) {
            closestDistance = distance;
            hitObject = object.get();
        }
    }

    outDistance = closestDistance;
    return hitObject;
}

std::array<int, 3> Renderer::ComputeShading(
    const IObject* hitObject,
    const math::Point3D& hitPoint,
    const core::Ray& ray,
    const std::vector<std::unique_ptr<ILight>>& lights)
{
    (void)ray;

    const math::Vector3D normal = hitObject->GetNormal(hitPoint).normalized();
    const std::array<int, 3> base = hitObject->GetColor();

    double ambientIntensity = 0.15;
    double diffuseIntensity = 0.0;

    for (const auto& light : lights) {
        if (!light)
            continue;
        if (light->GetType() == LightType::Directional) {
            const auto dir = light->GetDirection();
            const math::Vector3D lightDir = math::Vector3D(
                -dir[0], -dir[1], -dir[2]).normalized();
            const double diff = std::max(0.0, normal.dot(lightDir));
            diffuseIntensity += diff * light->GetIntensity();
        }
        if (light->GetType() == LightType::Ambient) {
            ambientIntensity += light->GetIntensity();
        }
    }

    if (lights.empty()) {
        const math::Vector3D fallbackDir = math::Vector3D(-0.4, -1.0, -0.6).normalized() * -1.0;
        diffuseIntensity = std::max(0.0, normal.dot(fallbackDir)) * 0.85;
    }

    const double intensity = std::clamp(ambientIntensity + diffuseIntensity, 0.0, 1.0);

    return {
        static_cast<int>(std::clamp(static_cast<double>(base[0]) * intensity, 0.0, 255.0)),
        static_cast<int>(std::clamp(static_cast<double>(base[1]) * intensity, 0.0, 255.0)),
        static_cast<int>(std::clamp(static_cast<double>(base[2]) * intensity, 0.0, 255.0))
    };
}

void Renderer::ComputePixel(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const std::vector<std::unique_ptr<ILight>>& lights,
    const render::Camera& camera,
    Tile& tile,
    std::size_t x,
    std::size_t y,
    std::size_t width)
{
    tile.SetState(COMPUTING);

    const core::Ray ray = GenerateRay(camera, x, y, width);
    double closestDistance = 0.0;
    const IObject* hitObject = FindClosestIntersection(objects, ray, closestDistance);

    if (!hitObject) {
        tile.SetColor(GenerateSkyColor(y, camera.resolution.second));
        tile.SetState(COMPUTED);
        return;
    }

    const math::Point3D hitPoint = ray.at(closestDistance);
    const std::array<int, 3> color = ComputeShading(hitObject, hitPoint, ray, lights);
    tile.SetColor(color);
    tile.SetState(COMPUTED);
}

} // namespace raytracer::threading