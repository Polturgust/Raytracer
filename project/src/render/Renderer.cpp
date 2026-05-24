/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** Renderer.cpp - Graphics/Ray computation implementation
*/

#include "Renderer.hpp"

#include <limits>

namespace raytracer::threading {

namespace {

constexpr double RAY_EPSILON = 0.001;
constexpr int MAX_RAY_DEPTH = 3;

std::array<int, 3> BlendColors(
    const std::array<int, 3>& base,
    const std::array<int, 3>& reflected,
    const std::array<int, 3>& refracted,
    const raytracer::Material& material)
{
    const auto clampColor = [](double value) -> int {
        if (value < 0.0)
            return 0;
        if (value > 255.0)
            return 255;
        return static_cast<int>(value);
    };

    const double reflective = std::clamp(material.reflection, 0.0, 1.0);
    const double transparent = std::clamp(material.transparency, 0.0, 1.0);
    const double localWeight = std::clamp(1.0 - reflective - transparent, 0.0, 1.0);

    return {
        clampColor(static_cast<double>(base[0]) * localWeight + static_cast<double>(reflected[0]) * reflective + static_cast<double>(refracted[0]) * transparent),
        clampColor(static_cast<double>(base[1]) * localWeight + static_cast<double>(reflected[1]) * reflective + static_cast<double>(refracted[1]) * transparent),
        clampColor(static_cast<double>(base[2]) * localWeight + static_cast<double>(reflected[2]) * reflective + static_cast<double>(refracted[2]) * transparent)
    };
}

raytracer::math::Vector3D Reflect(const raytracer::math::Vector3D& direction, const raytracer::math::Vector3D& normal)
{
    return direction - normal * (2.0 * direction.dot(normal));
}

bool Refract(
    const raytracer::math::Vector3D& direction,
    const raytracer::math::Vector3D& normal,
    double refractionIndex,
    raytracer::math::Vector3D& outDirection)
{
    if (refractionIndex <= 0.0)
        return false;

    raytracer::math::Vector3D unitDirection = direction.normalized();
    raytracer::math::Vector3D unitNormal = normal.normalized();
    double cosi = std::clamp(unitDirection.dot(unitNormal), -1.0, 1.0);
    double etai = 1.0;
    double etat = refractionIndex;

    if (cosi > 0.0) {
        std::swap(etai, etat);
        unitNormal = -unitNormal;
    } else {
        cosi = -cosi;
    }

    const double eta = etai / etat;
    const double k = 1.0 - eta * eta * (1.0 - cosi * cosi);
    if (k < 0.0)
        return false;

    outDirection = unitDirection * eta + unitNormal * (eta * cosi - std::sqrt(k));
    return true;
}

std::array<int, 3> BackgroundFromRay(const core::Ray& ray)
{
    const auto clampColor = [](double value) -> int {
        if (value < 0.0)
            return 0;
        if (value > 255.0)
            return 255;
        return static_cast<int>(value);
    };

    const double ratio = std::clamp((ray.direction.y + 1.0) * 0.5, 0.0, 1.0);
    return {
        clampColor(20.0 + 80.0 * ratio),
        clampColor(40.0 + 120.0 * ratio),
        clampColor(70.0 + 150.0 * ratio)
    };
}

}

std::array<int, 3> Renderer::GenerateSkyColor(std::size_t y, std::size_t height)
{
    const auto clampColor = [](double value) -> int {
        if (value < 0.0) return 0;
        if (value > 255.0) return 255;
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

    const double sx = ((static_cast<double>(x) + 0.5) / dWidth * 2.0 - 1.0) * aspect * imageScale;
    const double sy = (1.0 - (static_cast<double>(y) + 0.5) / height * 2.0) * imageScale;

    return core::Ray(origin, math::Vector3D(sx, sy, 1.0).normalized());
}

const IObject* Renderer::FindClosestIntersection(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const core::Ray& ray,
    double& outDistance)
{
    const IObject* hitObject = nullptr;
    double closestDistance = 1e30;

    for (const auto& object : objects) {
        if (!object) continue;
        double distance = 0.0;
        if (object->Intersect(ray, distance) && distance > 0.001 && distance < closestDistance) {
            closestDistance = distance;
            hitObject = object.get();
        }
    }
    outDistance = closestDistance;
    return hitObject;
}

bool Renderer::IsInShadow(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const math::Point3D& shadowOrigin,
    const math::Vector3D& lightDir,
    double maxDistance)
{
    const core::Ray shadowRay(shadowOrigin, lightDir);
    double shadowDist = 0.0;
    const IObject* blocker = FindClosestIntersection(objects, shadowRay, shadowDist);

    return blocker != nullptr && shadowDist < maxDistance;
}

void Renderer::ComputeLightContribution(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const ILight* light,
    const math::Vector3D& normal,
    const math::Point3D& hitPoint,
    const std::array<int, 3>& base,
    double& r,
    double& g,
    double& b)
{
    const std::array<int, 3> lc = light->GetColor();
    const double lr = lc[0] / 255.0;
    const double lg = lc[1] / 255.0;
    const double lb = lc[2] / 255.0;
 
    switch (light->GetType()) {
        case LightType::Ambient: {
            const double i = light->GetIntensity();
            r += base[0] * lr * i;
            g += base[1] * lg * i;
            b += base[2] * lb * i;
            break;
        }
        case LightType::Directional: {
            const auto dir = light->GetDirection();
            const math::Vector3D lightDir = math::Vector3D(
                -dir[0], -dir[1], -dir[2]).normalized();
            const double diff = std::max(0.0, normal.dot(lightDir));
            if (diff <= 0.0) break;
            const math::Point3D shadowOrigin = hitPoint + (normal * 0.001);
            if (IsInShadow(objects, shadowOrigin, lightDir, 1e30)) break;
            const double i = diff * light->GetIntensity();
            r += base[0] * lr * i;
            g += base[1] * lg * i;
            b += base[2] * lb * i;
            break;
        }
        case LightType::Point: {
            const auto pos = light->GetDirection();
            const math::Point3D lightPos(pos[0], pos[1], pos[2]);
            const math::Vector3D toLight = lightPos - hitPoint;
            const double distToLight = toLight.length();
            const math::Vector3D lightDir = toLight / distToLight;
            const double diff = std::max(0.0, normal.dot(lightDir));
            if (diff <= 0.0) break;
            const math::Point3D shadowOrigin = hitPoint + (normal * 0.001);
            if (IsInShadow(objects, shadowOrigin, lightDir, distToLight)) break;
            const double i = diff * light->GetIntensity();
            r += base[0] * lr * i;
            g += base[1] * lg * i;
            b += base[2] * lb * i;
            break;
        }
        default:
            break;
    }
}
 
std::array<int, 3> Renderer::ComputeShading(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const IObject* hitObject,
    const math::Point3D& hitPoint,
    const core::Ray& ray,
    const std::vector<std::unique_ptr<ILight>>& lights)
{
    (void)ray;

    const math::Vector3D normal = hitObject->GetNormal(hitPoint).normalized();
    const std::array<int, 3> base = hitObject->GetColor();
    double r = 0.0, g = 0.0, b = 0.0;
 
    if (lights.empty()) {
        const math::Vector3D fallbackDir = math::Vector3D(-0.4, -1.0, -0.6).normalized() * -1.0;
        const double intensity = 0.15 + std::max(0.0, normal.dot(fallbackDir)) * 0.85;
        r = base[0] * intensity;
        g = base[1] * intensity;
        b = base[2] * intensity;
    } else {
        for (const auto& light : lights) {
            if (!light) continue;
            ComputeLightContribution(objects, light.get(), normal, hitPoint, base, r, g, b);
        }
    }
 
    const auto clamp = [](double v) -> int {
        return static_cast<int>(std::clamp(v, 0.0, 255.0));
    };
    return {clamp(r), clamp(g), clamp(b)};
}

std::array<int, 3> Renderer::TraceRay(
    const std::vector<std::unique_ptr<IObject>>& objects,
    const core::Ray& ray,
    int depth,
    const std::vector<std::unique_ptr<ILight>>& lights)
{
    if (depth <= 0)
        return BackgroundFromRay(ray);

    double closestDistance = 0.0;
    const IObject* hitObject = FindClosestIntersection(objects, ray, closestDistance);
    if (!hitObject)
        return BackgroundFromRay(ray);

    const math::Point3D hitPoint = ray.at(closestDistance);
    const std::array<int, 3> localColor = ComputeShading(objects, hitObject, hitPoint, ray, lights);
    const Material material = hitObject->GetMaterial();

    if (material.reflection <= 0.0 && material.transparency <= 0.0)
        return localColor;

    math::Vector3D normal = hitObject->GetNormal(hitPoint).normalized();
    if (ray.direction.dot(normal) > 0.0)
        normal = -normal;

    const math::Vector3D reflectedDirection = Reflect(ray.direction.normalized(), normal).normalized();
    const core::Ray reflectedRay(hitPoint + normal * RAY_EPSILON, reflectedDirection);
    const std::array<int, 3> reflectedColor = TraceRay(objects, reflectedRay, depth - 1, lights);

    std::array<int, 3> refractedColor = BackgroundFromRay(ray);
    math::Vector3D refractedDirection;
    if (material.transparency > 0.0 && Refract(ray.direction, normal, material.refraction, refractedDirection)) {
        const core::Ray refractedRay(hitPoint - normal * RAY_EPSILON, refractedDirection.normalized());
        refractedColor = TraceRay(objects, refractedRay, depth - 1, lights);
    } else if (material.transparency > 0.0) {
        refractedColor = reflectedColor;
    }

    return BlendColors(localColor, reflectedColor, refractedColor, material);
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
    tile.SetColor(TraceRay(objects, ray, MAX_RAY_DEPTH, lights));
    tile.SetState(COMPUTED);
}

} // namespace raytracer::threading