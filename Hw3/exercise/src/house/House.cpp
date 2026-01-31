#include "House.h"

#include "Bathroom.h"
#include "Bedroom1.h"
#include "Bedroom2.h"
#include "DiningRoom.h"
#include "FamilyRoom.h"
#include "Garage.h"
#include "Hallway.h"
#include "Porch.h"
#include "../Model.h"
#include "../AssetLoader.h"
#include "../LightingManager.h"
#include "../GameManager.h"
#include "../Spline.h"

void House::setup() {
    // Porch *porch = new Porch(Vec3<GLfloat>(0, 0, 0), House::scale);
    // addChildren(porch);
    // porch->setStatic(true);

    // FamilyRoom *familyRoom = new FamilyRoom(Vec3<GLfloat>(Porch::secondLayerSpacing + Porch::frontLength + Porch::ceilingSpacing - House::ridgeThickness, 0, Porch::ceilingSpacing + Porch::secondLayerSpacing + Porch::leftLength - House::exteriorWallThickness), House::scale);
    // Vec3<GLfloat> familyRoomPosition = familyRoom->getPosition() + Vec3<GLfloat>(0, 0, -FamilyRoom::length);
    // familyRoom->setPosition(familyRoomPosition);
    // addChildren(familyRoom);
    // familyRoom->setStatic(true);

    // DiningRoom *diningRoom = new DiningRoom(familyRoom->getPosition() + Vec3<GLfloat>(FamilyRoom::totalWidth, 0, 0), House::scale);
    // Vec3<GLfloat> diningRoomPosition = diningRoom->getPosition() + Vec3<GLfloat>(0, 0, -(DiningRoom::totalLength - FamilyRoom::totalLength));
    // diningRoom->setPosition(diningRoomPosition);
    // addChildren(diningRoom);
    // diningRoom->setStatic(true);

    // Garage *garage = new Garage(diningRoom->getPosition() + Vec3<GLfloat>(DiningRoom::totalWidth - House::ridgeThickness, 0, -House::garageInset), scale);
    // Model *car = new Model(GameManager::getAssetPath("Car.obj"), Vec3(Garage::totalWidth / 2.5f, floorHeight + 0.2f, Garage::totalLength / 2.0f), false, {1.0f, 1.0f, 1.0f}, TextureID::CAR, MaterialID::MATTE);
    // car->setRotation(180, Vec3<GLfloat>(0.0f, 1.0f, 0.0f));
    // car->setStatic(true);
    // garage->addChildren(car);
    // garageCeiling = garage->getCeiling();
    // addChildren(garage);

    // Bathroom *bathroom = new Bathroom(familyRoom->getPosition(), House::scale);
    // bathroom->setPosition(bathroom->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bathroom::totalLength));
    // addChildren(bathroom);
    // bathroom->setStatic(true);

    // Bedroom2 *bedroom2 = new Bedroom2(bathroom->getPosition(), House::scale);
    // bedroom2->setPosition(bedroom2->getPosition() + Vec3<GLfloat>(0.0f, 0.0f, -Bedroom2::totalLength));
    // addChildren(bedroom2);
    // bedroom2->setStatic(true);

    // Bedroom1 *bedroom1 = new Bedroom1(bedroom2->getPosition() + Vec3<GLfloat>(Bedroom2::totalWidth, 0.0f, 0.0f), House::scale);
    // addChildren(bedroom1);
    // bedroom1->setStatic(true);

    // Hallway *hallway = new Hallway(diningRoomPosition, scale);
    // Vec3<GLfloat> hallwayPosition = hallway->getPosition() + Vec3<GLfloat>(-Hallway::totalWidth, 0, -House::interiorWallThickness);
    // hallway->setPosition(hallwayPosition);
    // addChildren(hallway);

    // Object *ceilingLight = AssetLoader::load(GameManager::getAssetPath("ceilingLight.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    // LightConfig config;
    // config.position = {0.0f, -1.05f, 0.0f};
    // config.color = {1.0f, 1.0f, 1.0f};
    // config.linear = 0.5f;
    // config.quadratic = 0.3f;
    // LightingManager::registerLight(config, ceilingLight);
    // addChildren(ceilingLight);


    // Object *bathroomMirror = AssetLoader::load(GameManager::getAssetPath("bathroommirror.txt"), Vec3<GLfloat>(0.0f, 0.0f, 0.0f));
    // LightConfig mirrorConfig;
    // mirrorConfig.position = {0.0f, 0.55f, 0.08f};
    // mirrorConfig.color = {1.0f, 1.0f, 1.0f};
    // mirrorConfig.linear = 0.7f;
    // mirrorConfig.quadratic = 0.4f;
    // LightingManager::registerLight(mirrorConfig, bathroomMirror);
    // addChildren(bathroomMirror);

    // // PATHWAY
    // GLfloat pathWidth = Garage::doorWidth;
    // GLfloat pathLength = 16;
    // GLfloat pathHeight = 0.02f;

    // GLfloat doorOffsetX = Garage::sidePanelWidth;
    // GLfloat garageFrontZ = Garage::ceilingOffset + Garage::length - 2 * House::exteriorWallThickness;
    // Vec3<GLfloat> pathPos = garage->getPosition() + Vec3(
    //     doorOffsetX,
    //     0.0f,
    //     garageFrontZ
    // );

    // AnchoredCuboid *pathway = new AnchoredCuboid(pathPos, Vec3(pathWidth, pathHeight, pathLength), false);
    // pathway->setTexture(TextureID::STONE, false);
    // pathway->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT));
    // pathway->setSubdivisions(10);
    // addChildren(pathway);
    // pathway->setStatic(true);

    // GLfloat branchLength = Garage::doorWidth * 2.5;
    // GLfloat branchWidth = Garage::doorWidth;
    // GLfloat branchZOffset = DiningRoom::totalLength - House::exteriorWallThickness - House::ridgeThickness - (Garage::length - House::garageInset) + Porch::frontLength;
    // AnchoredCuboid *pathway1 = new AnchoredCuboid(pathPos + Vec3<GLfloat>(-branchLength, 0.0f, branchZOffset), Vec3(branchLength, pathHeight, branchWidth), false);
    // pathway1->setTexture(TextureID::STONE, false);
    // pathway1->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT));
    // pathway1->setSubdivisions(10);
    // addChildren(pathway1);
    // pathway1->setStatic(true);

    // // ROOF
    // roof = new Object(Vec3<GLfloat>(bedroom2->getPosition().x, 0.0f, bedroom2->getPosition().z), House::scale);

    // GLfloat ceilingWidth = FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * (exteriorWallThickness + ridgeThickness);
    // GLfloat ceilingHeight = 0.2f;
    // GLfloat ceilingLength = DiningRoom::totalLength + Bedroom1::totalLength - 2 * (exteriorWallThickness + ridgeThickness);

    // roof->addChildren(new AnchoredCube(Vec3((exteriorWallThickness + ridgeThickness), House::height + House::floorHeight, (exteriorWallThickness + ridgeThickness)),
    // Vec3(ceilingWidth, ceilingHeight,  ceilingLength),
    //     gravity,
    //     House::darkColor
    // ));

    // roof->addChildren(new TriangularRidgedWall(Vec3(ridgeThickness, House::height + House::floorHeight, Bedroom1::totalLength + DiningRoom::totalLength - exteriorWallThickness - ridgeThickness),
    //     Vec3(FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * ridgeThickness, roofHeight, exteriorWallThickness),
    //     Vec3(0.0f, 0.0f, 1.0f),
    //     House::ridgeSpacing + 0.032,
    //     House::ridgeThickness,
    //     gravity,
    //     House::lightColor
    // ));

    // roof->addChildren(new TriangularRidgedWall(Vec3(ridgeThickness, House::height + House::floorHeight, ridgeThickness),
    //     Vec3(FamilyRoom::totalWidth + DiningRoom::totalWidth - 2 * ridgeThickness, roofHeight, exteriorWallThickness),
    //     Vec3(0.0f, 0.0f, -1.0f),
    //     House::ridgeSpacing + 0.032,
    //     House::ridgeThickness,
    //     gravity,
    //     House::lightColor
    // ));

    // roof->addChildren(AssetLoader::load(GameManager::getAssetPath("roof.txt"), Vec3<GLfloat>(-bedroom2->getPosition().x, 0.0f, -bedroom2->getPosition().z)));

    // addChildren(roof);

    // Object* faucet = AssetLoader::load(GameManager::getAssetPath("tap.txt"), Vec3<float>(0.0f, 0.3f, 0.0f)); 

    // std::vector<Vec3<float>> pathPoints;
    // pathPoints.push_back(Vec3<float>(0.0f, 1.025f, 0.35f)); 
    // pathPoints.push_back(Vec3<float>(0.0f, 0.90f, 0.38f)); 
    // pathPoints.push_back(Vec3<float>(0.0f, 0.40f, 0.40f)); 
    // pathPoints.push_back(Vec3<float>(0.0f, 0.05f, 0.45f)); 

    // //! Coiling on the Floor
    // float angle = 0.0f;
    // float radius = 0.5f;
    // for (int i = 0; i < 5; i++) {
    //     angle += 0.8f; 
    //     radius += 0.06f;
    //     float x = sin(angle) * radius;
    //     float z = cos(angle) * radius + 0.5f; 
    //     pathPoints.push_back(Vec3<float>(x, 0.05f, z));
    // }

    // //! Generate the 3D Tube Grid
    // std::vector<std::vector<Vec3<float>>> tubeGrid;
    // float hoseThickness = 0.035f; //! Radius of the hose
    // int ringRes = 6; //! How round the tube is (N sides)

    // for (size_t i = 0; i < pathPoints.size(); i++) {
    //     //! Calculate Direction
    //     Vec3<float> pCurrent = pathPoints[i];
    //     Vec3<float> pNext = (i < pathPoints.size() - 1) ? pathPoints[i + 1] : pathPoints[i];
    //     Vec3<float> pPrev = (i > 0) ? pathPoints[i - 1] : pathPoints[i];
        
    //     Vec3<float> tangent;
    //     if (i == 0) tangent = (pNext - pCurrent).normalize();
    //     else if (i == pathPoints.size() - 1) tangent = (pCurrent - pPrev).normalize();
    //     else tangent = (pNext - pPrev).normalize();

    //     //! Calculate Orientation Frame
    //     Vec3<float> worldUp = Vec3<float>(0.0f, 1.0f, 0.0f);
    //     if (fabs(tangent.y) > 0.99f) worldUp = Vec3<float>(1.0f, 0.0f, 0.0f);

    //     Vec3<float> right = tangent.cross(worldUp).normalize();
    //     Vec3<float> up = right.cross(tangent).normalize();

    //     //! Generate Ring of points
    //     std::vector<Vec3<float>> ring;
    //     for (int j = 0; j <= ringRes; j++) {
    //         float theta = (float)j / ringRes * 6.28318f;
    //         float cosT = cos(theta);
    //         float sinT = sin(theta);
            
    //         Vec3<float> point = pCurrent + (right * cosT * hoseThickness) + (up * sinT * hoseThickness);
    //         ring.push_back(point);
    //     }
    //     tubeGrid.push_back(ring);
    // }

    // Color3f hoseColor = {0.0f, 0.4f, 0.0f}; 
    // NurbsSurface* hose = new NurbsSurface(tubeGrid, Vec3<float>(0,0,0), 4, hoseColor, TextureID::GREEN_WOOL, MaterialID::MATTE);
    // hose->setStatic(true);
    // faucet->setStatic(true);

    // faucet->addChildren(hose);
    // addChildren(faucet);
}

Object *House::getGarageCeiling() {
    return garageCeiling;
}

Object *House::getRoof() {
    return roof;
}
