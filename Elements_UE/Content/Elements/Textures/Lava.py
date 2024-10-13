import unreal
import mm
from importlib import reload
reload(mm)
mat = mm.Material('Lava', True)
mat.clear()
TextureSample_Color = mat.add_node('TextureSample', -400, -176)
TextureSample_Color.set_editor_property('texture', mm.import_texture('B:/UnrealEngine5_Projects/_repos4/Elements/Elements_UE/Content/Elements/Textures/Lava_albedo.png', '/Game/Textures'))
mat.connect_property(TextureSample_Color, 'RGB', unreal.MaterialProperty.MP_BASE_COLOR)
TextureSample_ORM = mat.add_node('TextureSample', -400, 96)
TextureSample_ORM.set_editor_property('texture', mm.import_texture('B:/UnrealEngine5_Projects/_repos4/Elements/Elements_UE/Content/Elements/Textures/Lava_orm.png', '/Game/Textures'))
mat.connect_property(TextureSample_ORM, 'B', unreal.MaterialProperty.MP_METALLIC)
mat.connect_property(TextureSample_ORM, 'G', unreal.MaterialProperty.MP_ROUGHNESS)
mat.connect_property(TextureSample_ORM, 'R', unreal.MaterialProperty.MP_AMBIENT_OCCLUSION)
TextureSample_Normal = mat.add_node('TextureSample', -400, 368)
TextureSample_Normal.set_editor_property('texture', mm.import_texture('B:/UnrealEngine5_Projects/_repos4/Elements/Elements_UE/Content/Elements/Textures/Lava_normal.png', '/Game/Textures'))
mat.connect_property(TextureSample_Normal, 'RGB', unreal.MaterialProperty.MP_NORMAL)
MaterialFunctionCall_0 = mat.add_node('MaterialFunctionCall', -976, 32)
MaterialFunctionCall_0.set_editor_property('material_function', mm.get_object_from_path("/Script/Engine.MaterialFunction'/Engine/Functions/Engine_MaterialFunctions01/Texturing/ParallaxOcclusionMapping.ParallaxOcclusionMapping'"))
TextureObject_0 = mat.add_node('TextureObject', -1200, -96)
TextureObject_0.set_editor_property('texture', mm.import_texture('B:/UnrealEngine5_Projects/_repos4/Elements/Elements_UE/Content/Elements/Textures/Lava_height.png', '/Game/Textures'))
Constant_0 = mat.add_node('Constant', -1200, 80)
Constant_0.set_editor_property('r', 0.05)
Constant_1 = mat.add_node('Constant', -1200, 160)
Constant_1.set_editor_property('r', 8.0)
Constant_2 = mat.add_node('Constant', -1200, 240)
Constant_2.set_editor_property('r', 16.0)
Constant4Vector_0 = mat.add_node('Constant4Vector', -1200, 320)
mat.connect_nodes(MaterialFunctionCall_0, 'Parallax UVs', TextureSample_Color, '')
mat.connect_nodes(MaterialFunctionCall_0, 'Parallax UVs', TextureSample_ORM, '')
mat.connect_nodes(MaterialFunctionCall_0, 'Parallax UVs', TextureSample_Normal, '')
mat.connect_nodes(TextureObject_0, '', MaterialFunctionCall_0, 'Heightmap Texture')
mat.connect_nodes(Constant_0, '', MaterialFunctionCall_0, 'Height Ratio')
mat.connect_nodes(Constant_1, '', MaterialFunctionCall_0, 'Min Steps')
mat.connect_nodes(Constant_2, '', MaterialFunctionCall_0, 'Max Steps')
mat.connect_nodes(Constant4Vector_0, '', MaterialFunctionCall_0, 'Heightmap Channel')
mat.save()



