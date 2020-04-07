// cl /LD cutedge.cpp /I C:/Users/cactus/Arnold-5.2.2.0-windows/include C:/Users/cactus/Arnold-5.2.2.0-windows/lib/ai.lib /link /out:cutedge.dll


#include <ai.h>
 
AI_SHADER_NODE_EXPORT_METHODS(quantizeCutEdgeMethods);

struct ShaderData {
  AtString trace_set;
  AtVector cam_pos;
};


enum CutedgeParams {
  p_pos,
  p_dir,
  p_traceset
};
 
node_parameters {
  AiParameterVec("pos", 0.0, 0.0, 0.0);
  AiParameterVec("dir", 0.0, 0.0, 0.0);
  AiParameterStr("traceset", "");
}

node_initialize {
  ShaderData* data = new ShaderData;
  AiNodeSetLocalData(node, data);
}

node_update {
  ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
  data->trace_set = AiNodeGetStr(node, "traceset");
  
  AtNode *camera = AiUniverseGetCamera();
  const AtMatrix& cMat = AiNodeGetMatrix(camera, AtString("matrix"));
  data->cam_pos = {cMat[3][0], cMat[3][1], cMat[3][2]};
}

node_finish {
  ShaderData* data = (ShaderData*)AiNodeGetLocalData(node);
  AiNodeSetLocalData(node, NULL);
  delete data;
}
 
shader_evaluate {
  ShaderData* data = (ShaderData*)AiNodeGetLocalData(node);

  AtVector pos = AiShaderEvalParamVec(p_pos);
  AtVector dir = - AiShaderEvalParamVec(p_dir);

  if (data->trace_set.length()) {
    AiShaderGlobalsSetTraceSet(sg, data->trace_set, false);
  }

  AtVector p_orig = sg->P;

  AtRay ray_intersect_along_normal = AiMakeRay(AI_RAY_CAMERA, pos, &dir, AI_BIG, sg);
  AtScrSample hit = AtScrSample();

  if(AiTrace(ray_intersect_along_normal, AI_RGB_WHITE, hit)){
    AtVector hitpoint = hit.point;
    AtVector dir_cam_hit = AiV3Normalize(p_orig - data->cam_pos);

    AtRay ray_intersect_camera = AiMakeRay(AI_RAY_CAMERA, data->cam_pos, &dir_cam_hit, AI_BIG, sg);
    AtScrSample hit2 = AtScrSample();

    if (AiTrace(ray_intersect_camera, AI_RGB_WHITE, hit2)){
        if (hit2.obj != hit.obj){
            sg->out.RGB() = AI_RGB_BLACK;
        } else {
            sg->out.RGB() = AI_RGB_WHITE;
        }
    }
  }

  AiShaderGlobalsUnsetTraceSet(sg);
}
 
node_loader {
   if (i > 0) return false;
   node->methods     = quantizeCutEdgeMethods;
   node->output_type = AI_TYPE_RGB;
   node->name        = "quantize_cut_edge";
   node->node_type   = AI_NODE_SHADER;
   strcpy(node->version, AI_VERSION);
   return true;
}