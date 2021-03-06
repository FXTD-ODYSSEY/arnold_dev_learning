// https://docs.arnoldrenderer.com/display/A5ARP/Creating+a+Simple+Plugin
#include <ai.h>
 
AI_SHADER_NODE_EXPORT_METHODS(SimpleMethods);
 
enum SimpleParams { p_color };
 
node_parameters
{
   AiParameterRGB("color", 0.7f, 0.7f, 0.7f);
}
 
node_initialize{}
node_update{}
node_finish{}
 
shader_evaluate
{
   // sg 是指 AtShaderGlobals 类型
   sg->out.RGB() = AiShaderEvalParamRGB(p_color);
}
 
node_loader
{
   if (i > 0)
      return false;
   node->methods     = SimpleMethods;
   node->output_type = AI_TYPE_RGB;
   node->name        = "simple";
   node->node_type   = AI_NODE_SHADER;
   strcpy_s(node->version, AI_VERSION);
   return true;
}