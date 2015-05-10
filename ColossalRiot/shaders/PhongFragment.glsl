#version 330 core
/// @brief our output fragment colour
layout (location =0) out vec4 fragColour;
/// @brief[in] the vertex normal
in vec3 fragmentNormal;
/// @brief material structure
struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

// @brief light structure
struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation;
  float spotCosCutoff;
  float quadraticAttenuation;
  float linearAttenuation;
};

// @brief spot light structure
struct SpotLight{
    vec4 position;
    vec3 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float spotCosCutoff;
    float spotCosInnerCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

};

// @param material passed from our program
uniform Materials material;
// @param light passed from our program
uniform Lights light;
// @param spotlight passed from our program
uniform SpotLight spotLight;


in vec3 lightDir;

// out the blinn half vector
in vec3 halfVector;
in vec3 eyeDirection;
in vec3 vPosition;

/// @brief a function to compute point light values
vec4 spotLightCalc ()
{
                float nDotVP;       // normal * light direction
                float nDotR;        // normal * light reflection vector
                float pf=0;           // power factor
                float spotDot;      // cosine of angle between spotlight
                float spotAttenuation;     // spotlight attenuation factor;
                float attenuation;  // computed attenuation factor
                float d;            // distance from surface to light position
                vec3 VP;            // direction from surface to light position
                vec3 reflection;    // direction of maximum highlights

                // Compute vector from surface to light position
                VP = vec3 (spotLight.position) - vPosition;

                // Compute distance between surface and light position
                d = length (VP);

                // Normalize the vector from surface to light position
                VP = normalize (VP);

                // Compute attenuation
                attenuation = 1.f / (spotLight.constantAttenuation +
                                                                                                 spotLight.linearAttenuation * d +
                                                                                                 spotLight.quadraticAttenuation * d * d);

                // See if point on surface is inside cone of illumination
                spotDot = dot (-VP, normalize (spotLight.direction));

                if (spotDot < spotLight.spotCosCutoff)
                {
                                spotAttenuation = 0.f;
                }
                else
                {
                                // we are going to ramp from the outer cone value to the inner using
                                // smoothstep to create a smooth value for the falloff
                                float spotValue=smoothstep(spotLight.spotCosCutoff,spotLight.spotCosInnerCutoff,spotDot);
                                spotAttenuation = pow (spotValue, spotLight.spotExponent);
                }

                // Combine the spot and distance attenuation
                attenuation *= spotAttenuation;
                // calc the reflection for the highlight
                reflection = normalize (reflect (-normalize (VP), normalize(fragmentNormal)));

                nDotVP = max (0.f, dot (fragmentNormal, VP));
                nDotR = max (0.f, dot (normalize (fragmentNormal), reflection));

                pf=clamp(nDotVP,0.0,pow (nDotR, material.shininess));
                // combine the light / material values
                vec4 ambient = material.ambient * spotLight.ambient * attenuation;
                vec4 diffuse = material.diffuse * spotLight.diffuse * nDotVP * attenuation;
                vec4 specular = material.specular * spotLight.specular * pf * attenuation;

                return ambient + diffuse + specular;
}


vec4 pointLight()
{
  vec3 N = normalize(fragmentNormal);
  vec3 halfV;
  float ndothv;
  float attenuation;
  vec3 E = normalize(eyeDirection);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  vec4 diffuse=vec4(0);
  vec4 ambient=vec4(0);
  vec4 specular=vec4(0);
  if (lambertTerm > 0.0)
  {
  float d;            // distance from surface to light position
  vec3 VP;            // direction from surface to light position

  // Compute vector from surface to light position
  VP = vec3 (light.position) - vPosition;

  // Compute distance between surface and light position
    d = length (VP);


    diffuse+=material.diffuse*light.diffuse*lambertTerm;
    ambient+=material.ambient*light.ambient;
    halfV = normalize(halfVector);
    ndothv = max(dot(N, halfV), 0.0);
    specular+=material.specular*light.specular*pow(ndothv, material.shininess);
  }
    return ambient + diffuse + specular;
}



void main ()
{

fragColour=pointLight()+ spotLightCalc();
}

