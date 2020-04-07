// https://docs.arnoldrenderer.com/display/A5ARP/Trace+Sets

#include <ai.h>
 
AI_SHADER_NODE_EXPORT_METHODS(TracesetMethods);
 
node_parameters { }
node_initialize { }
node_update { }
node_finish { }
 
shader_evaluate
{
   const AtString probe_set("probe_set");
   const bool probe_set_inclusive = true;
 
   const AtString diffuse_set("diffuse_set");
   const bool diffuse_set_inclusive = true;
 
   AtRGB diffuse_color(0.5f, 0.5f, 0.5f);
 
   // trace probe ray along normal, turning the surface red on hits
   AiShaderGlobalsSetTraceSet(sg, probe_set, probe_set_inclusive);
   AtRay ray = AiMakeRay(AI_RAY_SHADOW, sg->P, &sg->Nf, AI_BIG, sg);
   AtScrSample sample;
   if (AiTrace(ray, AI_RGB_WHITE, sample))
      diffuse_color = AI_RGB_RED;
   AiShaderGlobalsUnsetTraceSet(sg);
 
   // diffuse BSDF
   AiShaderGlobalsSetTraceSet(sg, diffuse_set, diffuse_set_inclusive);
   sg->out.CLOSURE() = AiOrenNayarBSDF(sg, diffuse_color, sg->Nf);
   AiShaderGlobalsUnsetTraceSet(sg);
}
 
node_loader
{
   if (i > 0)
      return false;
 
   node->methods     = TracesetMethods;
   node->output_type = AI_TYPE_CLOSURE;
   node->name        = "traceset";
   node->node_type   = AI_NODE_SHADER;
   strcpy_s(node->version, AI_VERSION);
   return true;
}