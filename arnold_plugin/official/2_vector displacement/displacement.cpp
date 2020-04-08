/*
 * nonlinear noise displacement shader
 */
#include <ai.h>
#include <string.h>
 
enum
{
   p_octaves,
   p_freq,
   p_amplitude,
   p_bloom,
   p_type
};
 
AI_SHADER_NODE_EXPORT_METHODS(NonlinNzMethods);
 
#define ENUM_SCALAR_TYPES { "perlin", "abs_perlin", "recursive", "abs_recursive", NULL };
#define PERLIN        0
#define ABS_PERLIN    1
#define RECURSIVE     2
#define ABS_RECURSIVE 3
const char *types_enum[] = ENUM_SCALAR_TYPES;
 
node_parameters
{
   AiParameterInt ("octaves"  , 3);
   AiParameterFlt ("freq"     , 1);
   AiParameterFlt ("amplitude", 1);
   AiParameterFlt ("bloom"    , 1);  
   AiParameterEnum("type"     , PERLIN, types_enum);
}
 
float scalarfunc(AtVector P, int type, int octaves)
{
   float doubler = 1;
   float NzAccum = 0;
 
   switch (type)
   {
      case PERLIN:
         return AiPerlin3(P);
      case ABS_PERLIN:
         return fabs(AiPerlin3(P));
      case RECURSIVE:
         for (int i = 0; i < octaves; i++) {
            NzAccum += AiPerlin3(P*doubler) / doubler;
            doubler *= 2;
         }
         return NzAccum;
      case ABS_RECURSIVE:
         for (int i = 0; i < octaves; i++) {
            NzAccum += fabs(AiPerlin3(P*doubler)) / doubler;
            doubler *= 2;
         }
         return NzAccum;
   }
 
   return AiPerlin3(P);
}
 
shader_evaluate
{
   AtVector Ploc, Uloc, Vloc; // noise sample location, and over in U and V locations
   float Np, Nu, Nv;         // noise at P, noise at location over in U and V
   float Udelt, Vdelt;       // delta in the noise over in U and V
   float delta = .01;        // distance delta for noise samples
 
   int octaves = AiShaderEvalParamInt(p_octaves);
   float freq = AiShaderEvalParamFlt(p_freq);
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float bloom = AiShaderEvalParamFlt(p_bloom);
   int type = AiShaderEvalParamInt(p_type);
 
   AtVector U, V;
   if (sg->dPdu != AI_V3_ZERO && sg->dPdv != AI_V3_ZERO)
   {
      // tangents available, use them
      U = sg->dPdu;
      V = sg->dPdv;
   }
   else
   {
      // no tangents given, compute a pair
      AiV3BuildLocalFramePolar(U, V, sg->N);
   }
 
   if (type > ABS_PERLIN)
   {
      // adjust delta to highest frequency in recursive noise
      delta *= pow(.5,  octaves) * 2;
   }
   Ploc = sg->Po * freq;
   Uloc = Ploc + U * delta;
   Vloc = Ploc + V * delta;
 
   // noise sampled at P, and over in U and V
   Np = scalarfunc(Ploc, type, octaves);
   Nu = scalarfunc(Uloc, type, octaves);
   Nv = scalarfunc(Vloc, type, octaves);
 
   Udelt = (Nu - Np) * bloom;
   Vdelt = (Nv - Np) * bloom;
 
   AtVector Pstepped = sg->P;  
   int steps = 10;
   float stepscale = amplitude / steps;
 
   for (int i = 0; i < steps; i++)
   {
      // stepdir is the cross product of the derivatives
      AtVector stepdir = AiV3Cross(U,V);
      // deflect the derivatives
      U = AiV3Normalize(U + (stepdir * Udelt * stepscale));
      V = AiV3Normalize(V + (stepdir * Vdelt * stepscale));
      Pstepped += stepdir * Np * stepscale;
   }
 
   sg->out.VEC() = Pstepped - sg->P;
}
 
node_initialize
{
}
 
node_update
{
}
 
node_finish
{
}
 
node_loader
{
   if (i > 0) return false;
    
   node->methods      = NonlinNzMethods;
   node->output_type  = AI_TYPE_VECTOR;
   node->name         = "nonlinear_noise";
   node->node_type    = AI_NODE_SHADER;
   strcpy_s(node->version, AI_VERSION);
   return true;
}