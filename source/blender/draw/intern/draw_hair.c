/*
 * Copyright 2016, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s): Blender Institute
 *
 */

/** \file draw_hair.c
 *  \ingroup draw
 */

#include "BLI_utildefines.h"

#include "DNA_hair_types.h"
#include "DNA_scene_types.h"

#include "BKE_hair.h"

#include "DRW_render.h"

#include "GPU_extensions.h"
#include "GPU_texture.h"

#include "draw_common.h"

const char* DRW_hair_shader_defines(void)
{
	static char str[256];
	
	BLI_snprintf(str, sizeof(str), "#define HAIR_SHADER_FIBERS\n#define HAIR_SHADER_TEX_WIDTH %d\n",
	             GPU_max_texture_size());
	
	return str;
}

void DRW_hair_shader_uniforms(DRWShadingGroup *shgrp, Scene *UNUSED(scene),
                              GPUTexture **fibertex, const DRWHairFiberTextureBuffer *texbuffer)
{
	DRW_shgroup_uniform_vec2(shgrp, "viewport_size", DRW_viewport_size_get(), 1);
	//DRW_shgroup_uniform_float(shgrp, "ribbon_width", &tsettings->hair_draw_size, 1);
	static float test = 2.5f;
	DRW_shgroup_uniform_float(shgrp, "ribbon_width", &test, 1);
	
	DRW_shgroup_uniform_texture_ref(shgrp, "fiber_data", fibertex);
	DRW_shgroup_uniform_int(shgrp, "strand_map_start", &texbuffer->strand_map_start, 1);
	DRW_shgroup_uniform_int(shgrp, "strand_vertex_start", &texbuffer->strand_vertex_start, 1);
	DRW_shgroup_uniform_int(shgrp, "fiber_start", &texbuffer->fiber_start, 1);
}

void DRW_shgroup_hair(
        Object *ob,
        HairSystem *hsys,
        HairDrawSettings *draw_settings,
        struct DerivedMesh *scalp,
        DRWShadingGroup *shgrp_verts,
        DRWShadingGroup *UNUSED(shgrp_edges))
{
	switch (draw_settings->follicle_mode)
	{
		case HAIR_DRAW_FOLLICLE_NONE:
			break;
		case HAIR_DRAW_FOLLICLE_POINTS:
		{
			struct Gwn_Batch *geom = DRW_cache_hair_get_follicle_points(hsys, scalp);
			DRW_shgroup_call_add(shgrp_verts, geom, ob->obmat);
			break;
		}
		case HAIR_DRAW_FOLLICLE_AXES:
		{
			break;
		}
	}
}
