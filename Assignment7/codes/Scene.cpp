//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// only used in function Vector3f Scene::castRay(const Ray &ray, int depth) const
void negToZero(Vector3f &v) {
        v.x = v.x > 0 ? v.x : 0;
        v.y = v.y > 0 ? v.y : 0;
        v.z = v.z > 0 ? v.z : 0;
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection intersection = intersect(ray);

    if(!intersection.happened){
        return Vector3f(0);
    }  
    if(intersection.m->hasEmission()){
        if(depth == 0){
            return intersection.m->getEmission();
        } else {
            return Vector3f(0);
        }
        
    }

    Vector3f &p = intersection.coords;
    Vector3f wo = normalize(-ray.direction);
    Vector3f N = normalize(intersection.normal);
    Material* &m = intersection.m;
    
    // direct
    Vector3f L_dir(0); 

    Intersection inter; 
    float pdf_light = 0.0f;
    sampleLight(inter, pdf_light);

    Vector3f &x = inter.coords;
    Vector3f ws = normalize(x - p);
    Vector3f NN = normalize(inter.normal);

    auto block = intersect(Ray(p, ws));
    if(block.happened && (block.coords-x).norm() < 0.01) 
    {
        L_dir = inter.emit * m->eval(ws, wo, N) * dotProduct(ws, N) 
            * dotProduct(NN, -ws) / (dotProduct((x-p), (x-p)) * pdf_light);
        negToZero(L_dir);
    }

    // indirect
    Vector3f L_indir;

    float RR = this->RussianRoulette;
    if(get_random_float() < RR) 
    {
        Vector3f wi = normalize(m->sample(wo, N));
        L_indir = castRay(Ray(p, wi), depth+1) * m->eval(wi, wo, N) * dotProduct(wi, N)
            / (m->pdf(wi, wo, N) * RR);
        negToZero(L_indir);
    }
    
    return L_dir + L_indir;

}