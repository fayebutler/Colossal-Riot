#include "Squad.h"

ngl::Vec3 Squad::s_nextSelectionColour = ngl::Vec3(0.0,0.0,0.0);

Squad::Squad(GameWorld* world, int squadSize, ngl::Vec3 pos, float r):Vehicle(world, pos, ngl::Vec3(0,0,0), 0.0f, 1.0f, 10.0f,1.0f, 1.0f, 0.5f)
{
//    m_squadPos = ngl::Vec3(2.0,0,2.0);
    //m_squadPos = pos;

    //m_boundingRad = r;

    m_allArrived = false;
    m_foundWall = false;
    m_squadSize = squadSize;
    m_pathIndex =0;

    m_squadColour = ngl::Colour(1.0f,1.0f,0.0f,1.0f);

    m_squadRadius = squadSize*m_boundingRadius;

    for (int i = 0; i < squadSize; ++i)
    {
      Police* newPolice = new Police(world);
      newPolice->setBoudingRadius(m_boundingRadius);
      newPolice->setDetectionRadius(3.5f);
      newPolice->setPos(ngl::Vec3((((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_x, 0.0f, (((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_z));
      newPolice->setSquadPos(m_pos);
      newPolice->setSquadRadius(m_squadRadius);
      newPolice->setSquadID(m_ID);
      m_squadPolice.push_back(newPolice);
      m_policeArrived.push_back(false);

    }

    m_selectionColour = s_nextSelectionColour;
//    std::cout<<"SELECTION COLOUR "<<m_selectionColour.m_x<<" "<<m_selectionColour.m_y<<" "<<m_selectionColour.m_z<<std::endl;

    s_nextSelectionColour.m_x += 0.1;

    if(s_nextSelectionColour.m_x >= 1)
    {
        s_nextSelectionColour.m_x = 0;
        s_nextSelectionColour.m_y += 0.1;

        if(s_nextSelectionColour.m_y >= 1)
        {
            s_nextSelectionColour.m_y = 0;
            s_nextSelectionColour.m_z += 0.1;
        }
    }
}

ngl::Vec3 Squad::averagePolicePos()
{
    ngl::Vec3 currentTotal =0;
    for (int i = 0; i< m_squadSize; i++)
    {
        Police* currentPolice = m_squadPolice[i];
        currentTotal += currentPolice->getPos();
    }
     currentTotal /= m_squadSize;
    return currentTotal;
}

void Squad::update(double timeElapsed, double currentTime)
{
    // individual police loop
    if(m_path.size() == 0)
    {
        this->formWall();
    }
    for(unsigned int i=0; i<m_squadSize; ++i)
    {
        Police* currentPolice = m_squadPolice[i];
        currentPolice->setSquadPos(m_pos);
        currentPolice->setSquadRadius(m_squadRadius);


        if(m_path.size() != 0)
        {
//            std::cout<<"EN ROUUTTEEEE"<<std::endl;

            m_pos = averagePolicePos();

            if ((currentPolice->getPos()-m_path.back()).lengthSquared()<16)
            {
                    currentPolice->setIsMoving(false);
                    currentPolice->setPathIndex(0);
                    m_policeArrived[i]=true;
            }

            currentPolice->setCrosshair(m_path[currentPolice->getPathIndex()]);

           if((currentPolice->getPos() - currentPolice->getCrosshair()).lengthSquared() < 4)
           {
               currentPolice->setPathIndex(currentPolice->getPathIndex()+1);
           }

        }
        currentPolice->update(timeElapsed, currentTime);
    }

    for (int i=0; i<m_squadSize; ++i )
    {
//        std::cout<<"police"<<i<<" = "<<m_policeArrived[i]<<std::endl;
    }
    // a loop
    if(m_path.size() != 0)
    {
        for(unsigned int i=0; i<m_squadSize; ++i)
        {
            if(m_policeArrived[i]==false)
            {
//                std::cout<<"I'm false!!! "<<i<<" = "<<m_policeArrived[i]<<std::endl;
                m_allArrived = false;
                return;
            }
        }
//        std::cout<<"we're all here yay"<<std::endl;
        m_allArrived = true;
        m_path.clear();
    }

}

void Squad::draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
    for(unsigned int i=0; i<m_squadPolice.size(); ++i)
    {
        Police* currentPolice = m_squadPolice[i];
        currentPolice->draw(cam, mouseGlobalTX);
    }

    loadMatricesToShader(cam, mouseGlobalTX);

    ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,120);
    ngl::VAOPrimitives::instance()->draw("squad");

}

void Squad::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(m_squadColour));
    m.loadToShader("material");

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    ngl::Transformation trans;
    trans.setPosition(m_pos.m_x, 0.3, m_pos.m_z);
    trans.setRotation(90.0,0.0,0.0);


    M=trans.getMatrix()*mouseGlobalTX;
    MV=  M*cam->getViewMatrix();
    MVP= M*cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
//    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//    shader->setShaderParamFromMat4("M",M);

}

bool Squad::checkSelectionColour(const ngl::Vec3 colour)
{
    if(m_selectionColour.m_x >= colour.m_x-0.01f && m_selectionColour.m_x <= colour.m_x+0.01f &&
            m_selectionColour.m_y >= colour.m_y-0.01f && m_selectionColour.m_y <= colour.m_y+0.01f &&
            m_selectionColour.m_z >= colour.m_z-0.01f && m_selectionColour.m_z <= colour.m_z+0.01f)
    {
//        std::cout<<"MATCHES!!";
//        std::cout<<"SQUAD ID "<<getID()<<std::endl;

        return true;
    }
    else
    {
//        std::cout<<"DOESNT MATCH!";
//        std::cout<<"SQUAD ID "<<getID()<<std::endl;
        return false;
    }
}

void Squad::selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglColourShader"]->use();
    shader->setShaderParam4f("Colour", m_selectionColour.m_x, m_selectionColour.m_y,m_selectionColour.m_z, 1);

    ngl::Transformation t;
    t.setPosition(m_pos);
    t.setRotation(90.0,0.0,0.0);

    ngl::Mat4 M;
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
//    ngl::Mat3 normalMatrix;

    M=t.getMatrix()*mouseGlobalTX;
    MV=  M*cam->getViewMatrix();
    MVP= M*cam->getVPMatrix();

//    normalMatrix=MV;
//    normalMatrix.inverse();
//    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
//    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

//    shader->setShaderParamFromMat4("M",M);
    ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,120);
    ngl::VAOPrimitives::instance()->draw("squad");


}

void Squad::setPath(std::vector<ngl::Vec3> _path)
{
    m_path.clear();
    m_path = _path;
    for (int i=0; i<m_squadSize; i++)
    {
        m_policeArrived[i] = false;
        m_squadPolice[i]->setIsMoving(true);
        m_squadPolice[i]->setPathIndex(0);
    }
}


bool Squad::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:

    return true;

  default:
    std::cout<<"Agent: Message type not defined"<<std::endl;
    return false;
  }
}



void Squad::findClosestWalls(Squad* squad)
{
    m_closestWalls.clear();
    std::vector<Wall> inWalls = squad->getCurrentCell().getWalls();
    std::vector<int> wallMemory;
    int numberOfWallsToCheck = inWalls.size();

    std::cout<<"number of walls to check = "<<inWalls.size()<<std::endl;
    while (m_closestWalls.size() < numberOfWallsToCheck)
    {
        float shortestDist;
        Wall closestWall;
        int closestWallID = 0;
        bool isInMemory = false;
        for (int i = 0; i < inWalls.size(); i++ )
        {
            shortestDist = 10000000000.0f;
            isInMemory = false;
            for (std::vector<int>::iterator iter = wallMemory.begin(); iter != wallMemory.end(); ++iter)
            {

                if( *iter == i)
                {
                    isInMemory = true;
                    break;
                }

            }
            if(isInMemory == true){break;}
            Wall currentWall = inWalls[i];
            ngl::Vec3 currentCenter = (currentWall.start+currentWall.end)/2;

            ngl::Vec3 distance = (currentCenter-m_pos);

            if(distance.lengthSquared() < shortestDist)
            {
                shortestDist = distance.lengthSquared();
                closestWall = inWalls[i];
                closestWallID = i;
            }
        }
        wallMemory.push_back(closestWallID);
        m_closestWalls.push_back(closestWall);

    }
    inWalls.clear();
    wallMemory.clear();
//    return m_closestWalls;

}


void Squad::formWall()
{
//    findClosestWalls(this);

    int numberOfWallsToCheck =0;
    if(m_foundWall == false)
    {
        numberOfWallsToCheck = m_closestWalls.size();
        findClosestWalls(this);
    }
//bool foundWall = false;

//    if(m_foundWall == false)
//    {
//         findClosestWalls(this);
    for(int i=0; i<numberOfWallsToCheck; i++)
    {
        if(m_foundWall == false)
        {
            Wall currentWall = m_closestWalls[i];
            ngl::Vec3 currentNormal = currentWall.normal;
            ngl::Vec3 currentCenter = (currentWall.start+currentWall.end)/2;
            for(int j=0; j<numberOfWallsToCheck; j++)
            {
                if(i!=j)
                {
                    Wall testWall = m_closestWalls[j];
                    ngl::Vec3 testNormal = testWall.normal;
                    ngl::Vec3 testCenter = (testWall.start+testWall.end)/2;
                   // std::cout<<"test normals = "<<testNormal.m_x<<" "<<testNormal.m_y<<" "<<testNormal.m_z<<std::endl;
                    //std::cout<<"current normals = "<<currentNormal.m_x<<" "<<currentNormal.m_y<<" "<<currentNormal.m_z<<std::endl;

                    if(currentNormal + testNormal == ngl::Vec3(0.0,0.0,0.0))
                    {
                        //if vertical line
                        if(currentCenter.m_x - testCenter.m_x == 0)
                        {
                            ngl::Vec3 distance = currentCenter - testCenter;
                            ngl::Vec3 upperCenter = ngl::Vec3(0.0,0.0,0.0);

                            if(currentCenter.m_z < testCenter.m_z)
                            {
                                upperCenter = currentCenter;
                            }
                            else if(currentCenter.m_z > testCenter.m_z)
                            {
                                upperCenter = testCenter;
                            }
                            float dist = distance.length();
                            float numberOf= dist/m_boundingRadius;
                            std::cout<<"upperCenter = "<<upperCenter.m_x <<std::endl;

                            if(numberOf >= m_squadSize)
                            {
                                //send as many police to this
                                float spacing = dist/(m_squadSize*2);
                                std::vector<ngl::Vec3> positions;
                                positions.clear();
                                for(int i =0; i< m_squadSize*2; i+=2)
                                {
                                    ngl::Vec3 position = ngl::Vec3(upperCenter.m_x, 0.0, upperCenter.m_z +((i+1)*spacing));

                                    positions.push_back(position);
                                }
                                for(int i =0; i <m_squadSize; i++)
                                {
                                    Police* policeman = m_squadPolice[i];
                                    ngl::Vec3 position = positions[i];
                                    policeman->setPos(position);
                                    //policeman->setCrosshair(position);
                                   // Steering()->ArriveOn();
                                   // Steering()->SeekOn();
                                }

                             }
                             m_foundWall = true;
                             break;

                          }

                          else if(currentCenter.m_z - testCenter.m_z == 0)
                          {
                            //if horizontal line
                            ngl::Vec3 distance = currentCenter - testCenter;
                            ngl::Vec3 leftCenter = ngl::Vec3(0.0,0.0,0.0);
                            if(currentCenter.m_x < testCenter.m_x)
                            {
                                leftCenter = currentCenter;
                            }
                            else if(currentCenter.m_x > testCenter.m_x)
                            {
                                leftCenter = testCenter;
                            }
                            float dist = distance.length();
                            float numberOf= dist/m_boundingRadius;

                            if(numberOf >= m_squadSize)
                            {
                                //send as many police to this
                                float spacing = dist/(m_squadSize*2);
                                std::vector<ngl::Vec3> positions;
                                positions.clear();
                                for(int i =0; i< m_squadSize*2; i+=2)
                                {
                                    ngl::Vec3 position = ngl::Vec3(leftCenter.m_x +((i+1)*spacing), 0.0, leftCenter.m_z);
                                    positions.push_back(position);
                                }
                                for(int i =0; i <m_squadSize; i++)
                                {
                                    Police* policeman = m_squadPolice[i];
                                    ngl::Vec3 position = positions[i];
                                    policeman->setPos(position);
                                }
                             }
                            m_foundWall = true;
                            break;

                          }
                          else
                          {
                            m_foundWall = false;
                            std::cout<<" CANT FORM WALL "<<std::endl;
                          }


                     }

                  }

             }


          }


      }

   m_foundWall = false;

}


