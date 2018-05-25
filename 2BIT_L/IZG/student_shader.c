/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici
  /// čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních
  /// atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret*
  /// definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce
  /// shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()

    Uniforms uniforms;
    UniformLocation viewMatrixLocation, projectionMatrixLocation;
    Mat4 *viewMatrix, *projectionMatrix, mat;
    Vec4 posVec4;
    Vec3 *pos, *normal, *pos2, *normal2;

    uniforms = gpu_getUniformsHandle(gpu);

    viewMatrixLocation = getUniformLocation(gpu, "viewMatrix");
    projectionMatrixLocation = getUniformLocation(gpu, "projectionMatrix");

    viewMatrix = shader_interpretUniformAsMat4(uniforms, viewMatrixLocation);
    projectionMatrix = shader_interpretUniformAsMat4(uniforms, projectionMatrixLocation);

    pos = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
    normal = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);

    multiply_Mat4_Mat4(&mat, projectionMatrix, viewMatrix);
  	copy_Vec3Float_To_Vec4(&posVec4, pos, 1.0f);
  	multiply_Mat4_Vec4(&output->gl_Position, &mat, &posVec4);

  	pos2 = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  	normal2 = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);

  	init_Vec3(pos2, pos->data[0], pos->data[1], pos->data[2]);
  	init_Vec3(normal2, normal->data[0], normal->data[1], normal->data[2]);
}

void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()

    Uniforms uniforms;
    UniformLocation cameraPositionLocation, lightPositionLocation;
    Vec3 *camera, *lightSource, *posVecOut, *posVecNormal, dM, sM, vec3_1, vec3_2,
          N, L, V, R, finalVec;
    float val1, val2;
    uint8_t i;

    init_Vec3(&dM, 0.0f, 1.0f, 0.0f);
    init_Vec3(&sM, 1.0f, 1.0f, 1.0f);

    posVecOut = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  	posVecNormal = fs_interpretInputAttributeAsVec3(gpu, input, 1);

    uniforms = gpu_getUniformsHandle(gpu);

  	cameraPositionLocation = getUniformLocation(gpu, "cameraPosition");
    lightPositionLocation = getUniformLocation(gpu, "lightPosition");

  	camera = shader_interpretUniformAsVec3(uniforms, cameraPositionLocation);
  	lightSource = shader_interpretUniformAsVec3(uniforms, lightPositionLocation);

  	sub_Vec3(&vec3_1, lightSource, posVecOut);
  	sub_Vec3(&vec3_1, camera, posVecOut);

  	normalize_Vec3(&L, &vec3_1);
  	normalize_Vec3(&N, posVecNormal);
  	normalize_Vec3(&V, &vec3_1);

  	val1 = (dot_Vec3(&N, &L) < 1.0f
                ? dot_Vec3(&N, &L) > 0.0f ? dot_Vec3(&N, &L) : 0.0f
                : dot_Vec3(&N, &L) < 1.0f ? dot_Vec3(&N, &L) : 1.0f);

  	multiply_Vec3_Float(&vec3_1, &V, -1.0f);
  	reflect(&R, &vec3_1, &N);
  	normalize_Vec3(&R, &R);

  	val2 = (float)pow((dot_Vec3(&R, &L) < 1.0f
                ? dot_Vec3(&N, &L) > 0.0f ? dot_Vec3(&N, &L) : 0.0f
                : dot_Vec3(&N, &L) < 1.0f ? dot_Vec3(&N, &L) : 1.0f)
                , 40.0f);

  	multiply_Vec3_Float(&vec3_1, &dM, val1);
  	multiply_Vec3_Float(&vec3_2, &sM, val2 * 0.5f);
  	add_Vec3(&finalVec, &vec3_1, &vec3_2);

    for (i = 0; i < 3; i++)
  	 output->color.data[i] = finalVec.data[i];
    output->color.data[3] = 1.0f;
}

/// @}
