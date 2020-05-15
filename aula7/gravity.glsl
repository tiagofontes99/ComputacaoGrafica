in vec2 position;
in vec2 velocity;
in vec2 originalPos;

out vec2 outPosition;
out vec2 outVelocity;

uniform float time;
uniform vec2 mousePos;

void main()
{
    // Points move towards their original position...
    vec2 newVelocity = originalPos - position;

    // ... unless the mouse is nearby. In that case, they move towards the mouse.
    if (length(mousePos - originalPos) < 0.75f) {
        vec2 acceleration = 1.5f * normalize(mousePos - position);
        newVelocity = velocity + accelaration*time; // kinematics eq #TODO
    }
    
    if (length(newVelocity) > 1.0f)
        newVelocity = normalize(newVelocity);

    vec2 newPosition = velocity*time+1/2*accelaration*time*time ;  // kinematics eq #TODO
    outPosition = newPosition;
    outVelocity = newVelocity;
    gl_Position = vec4(newPosition, 0.0, 1.0);
}