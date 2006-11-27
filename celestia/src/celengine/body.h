// body.h
//
// Copyright (C) 2001-2006 Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _CELENGINE_BODY_H_
#define _CELENGINE_BODY_H_

#include <string>
#include <vector>
#include <map>
#include <celutil/utf8.h>
#include <celmath/quaternion.h>
#include <celengine/surface.h>
#include <celengine/atmosphere.h>
#include <celengine/orbit.h>
#include <celengine/star.h>
#include <celengine/location.h>
#include <celengine/rotation.h>

class ReferenceFrame;
class Body;

class PlanetarySystem
{
 public:
    PlanetarySystem(Body* _primary);
    PlanetarySystem(Star* _star);
    
    Star* getStar() const { return star; };
    Body* getPrimaryBody() const { return primary; };
    int getSystemSize() const { return satellites.size(); };
    Body* getBody(int i) const { return satellites[i]; };
    void addBody(Body* body);
    void removeBody(Body* body);
    void replaceBody(Body* oldBody, Body* newBody);

    enum TraversalResult
    {
        ContinueTraversal   = 0,
        StopTraversal       = 1
    };

    typedef bool (*TraversalFunc)(Body*, void*);

    bool traverse(TraversalFunc, void*) const;
    Body* find(const std::string&, bool deepSearch = false, bool i18n = false) const;
    std::vector<std::string> getCompletion(const std::string& _name, bool rec = true) const;

 private:
    typedef std::map<std::string, Body*, UTF8StringOrderingPredicate> ObjectIndex;

 private:
    Star* star;
    Body* primary;
    std::vector<Body*> satellites;
    ObjectIndex objectIndex;  // index of bodies by name
};


class RingSystem 
{
 public:
    float innerRadius;
    float outerRadius;
    Color color;
    MultiResTexture texture;

    RingSystem(float inner, float outer) :
        innerRadius(inner), outerRadius(outer), color(1.0f, 1.0f, 1.0f), texture()
        { };
    RingSystem(float inner, float outer, Color _color, int _loTexture = -1, int _texture = -1) :
        innerRadius(inner), outerRadius(outer), color(_color), texture(_loTexture, _texture)
        { };
    RingSystem(float inner, float outer, Color _color, const MultiResTexture& _texture) :
        innerRadius(inner), outerRadius(outer), color(_color), texture(_texture)
        { };
};


class Body
{
 public:
    Body(PlanetarySystem*);
    ~Body();

    enum
    {
        Planet      =    0x01,
        Moon        =    0x02,
        Asteroid    =    0x04,
        Comet       =    0x08,
        Spacecraft  =    0x10,
        Invisible   =    0x20,
        Barycenter  =    0x40,
        SmallBody   =    0x80,
        DwarfPlanet =   0x100,
        Unknown     = 0x10000,
    };

    PlanetarySystem* getSystem() const;
    std::string getName(bool i18n = false) const;
    void setName(const std::string);
    Orbit* getOrbit() const;
    void setOrbit(Orbit*);
    const Body* getOrbitBarycenter() const;
    void setOrbitBarycenter(const Body*);

    const ReferenceFrame* getOrbitFrame() const;
    void setOrbitFrame(const ReferenceFrame* f);
    const ReferenceFrame* getBodyFrame() const;
    void setBodyFrame(const ReferenceFrame* f);

    const RotationModel* getRotationModel() const;
    void setRotationModel(const RotationModel*);
    float getRadius() const;
    void setRadius(float);
    float getMass() const;
    void setMass(float);
    float getOblateness() const;
    void setOblateness(float);
    float getAlbedo() const;
    void setAlbedo(float);
    Quatf getOrientation() const;
    void setOrientation(const Quatf&);
    int getClassification() const;
    void setClassification(int);
    std::string getInfoURL() const;
    void setInfoURL(const std::string&);

    PlanetarySystem* getSatellites() const;
    void setSatellites(PlanetarySystem*);

    float getBoundingRadius() const;

    RingSystem* getRings() const;
    void setRings(const RingSystem&);
    const Atmosphere* getAtmosphere() const;
    Atmosphere* getAtmosphere();
    void setAtmosphere(const Atmosphere&);

    void setModel(ResourceHandle);
    ResourceHandle getModel() const;
    void setSurface(const Surface&);
    const Surface& getSurface() const;
    Surface& getSurface();

    float getLuminosity(const Star& sun,
                        float distanceFromSun) const;
    float getLuminosity(float sunLuminosity,
                        float distanceFromSun) const;
    float getApparentMagnitude(const Star& sun,
                               float distanceFromSun,
                               float distanceFromViewer) const;
    float getApparentMagnitude(const Star& sun,
                               const Vec3d& sunPosition,
                               const Vec3d& viewerPosition) const;
    float getApparentMagnitude(float sunLuminosity,
                               const Vec3d& sunPosition,
                               const Vec3d& viewerPosition) const;

    Mat4d getLocalToHeliocentric(double) const;
    Point3d getHeliocentricPosition(double) const;
    Quatd getEquatorialToBodyFixed(double) const;
    Quatd getEclipticalToEquatorial(double) const;
    Quatd getEclipticalToBodyFixed(double) const;
    Mat4d getBodyFixedToHeliocentric(double) const;

    Vec3f planetocentricToCartesian(float lon, float lat, float alt) const;
    Vec3f planetocentricToCartesian(const Vec3f& lonLatAlt) const;
    Vec3f cartesianToPlanetocentric(const Vec3f& v) const;

    bool extant(double) const;
    void setLifespan(double, double);
    void getLifespan(double&, double&) const;

    Surface* getAlternateSurface(const std::string&) const;
    void addAlternateSurface(const std::string&, Surface*);
    std::vector<std::string>* getAlternateSurfaceNames() const;

    std::vector<Location*>* getLocations() const;
    void addLocation(Location*);
    Location* findLocation(const std::string&, bool i18n = false) const;
    void computeLocations();

 private:
    std::string name;
    std::string i18nName;

    PlanetarySystem* system;
    
    Orbit* orbit;
    const Body* orbitBarycenter;
    const ReferenceFrame* orbitFrame;
    const ReferenceFrame* bodyFrame;
    
    const RotationModel* rotationModel;

    float radius;
    float mass;
    float oblateness;
    float albedo;
    Quatf orientation;

    double protos;
    double eschatos;

    ResourceHandle model;
    Surface surface;

    Atmosphere* atmosphere;
    RingSystem* rings;

    PlanetarySystem* satellites;

    int classification;

    std::string infoURL;

    typedef std::map<std::string, Surface*> AltSurfaceTable;
    AltSurfaceTable *altSurfaces;

    std::vector<Location*>* locations;
    mutable bool locationsComputed;
};

#endif // _CELENGINE_BODY_H_
