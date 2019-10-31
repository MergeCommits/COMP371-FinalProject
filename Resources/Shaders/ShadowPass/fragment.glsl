#version 330 core

in vec3 fsPosition;
in vec3 fsNormal;
in vec2 fsUV;
in vec4 fsPositionLightSpace;

uniform sampler2D shadowMap;
uniform sampler2D diffuse;

uniform bool enableShadows;
uniform bool enableTextures;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec4 fsColor;

out vec4 outColor;

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection) {
    // Normalize coordinates.
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform them to the [0,1] range.
    projCoords = projCoords * 0.5f + 0.5f;
    
    // Keep the shadow at 0.0 when outside the light's frustum.
    if (projCoords.z > 1.0f) {
        return 0.0f;
    }
    
    // Get the closest depth value from the light's perspective.
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // Get the depth of current fragment from light's perspective.
    float currentDepth = projCoords.z;
    
    // Calculate bias.
    float bias = max(0.05f * (1.0f - dot(normal, lightDirection)), 0.005f);
    
    // Check whether the current fragment position is in shadow.
    // Use PCF to produce softer edges on shadows.
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    shadow /= 9.0;
        
    return shadow;
}

void main() {
    // Run through the Phong illumination model.
    vec3 color = enableTextures ? texture(diffuse, fsUV).xyz : fsColor.xyz;
    vec3 normal = normalize(fsNormal);
    vec3 lightColor = vec3(1.0f);
    
    // Ambient color.
    vec3 ambientColor = vec3(0.15f);
    
    // Diffuse color.
    vec3 lightDirection = normalize(lightPosition - fsPosition);
    float phongDiff = max(dot(lightDirection, normal), 0.0f);
    vec3 diffuseColor = phongDiff * lightColor;
    
    // Specular color.
    vec3 viewDirection = normalize(cameraPosition - fsPosition);
    vec3 reflectOverNormal = reflect(-lightDirection, normal);
    float phongSpec = dot(viewDirection, reflectOverNormal);
    
    vec3 specularColor = vec3(0.0f);
    if (phongSpec > 0.0f) {
        float shineCoefficient = 32.0f;
        phongSpec = pow(phongSpec, shineCoefficient);
        specularColor = phongSpec * lightColor;
    }
    
    // Calculate shadow.
    float shadow = 0.0f;
    if (enableShadows) {
        shadow = shadowCalculation(fsPositionLightSpace, normal, lightDirection);
    }
    vec3 lighting = (ambientColor + (1.0f - shadow) * (diffuseColor + specularColor)) * color;
    
    outColor = vec4(lighting, 1.0f);
}
