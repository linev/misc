import { REVISION } from '../../threejs/src/constants.js';

export { WebGLMultipleRenderTargets } from '../../threejs/src/renderers/WebGLMultipleRenderTargets.js';
export { WebGLMultisampleRenderTarget } from '../../threejs/src/renderers/WebGLMultisampleRenderTarget.js';
export { WebGLCubeRenderTarget } from '../../threejs/src/renderers/WebGLCubeRenderTarget.js';
export { WebGLRenderTarget } from '../../threejs/src/renderers/WebGLRenderTarget.js';
export { WebGLRenderer } from '../../threejs/src/renderers/WebGLRenderer.js';
export { WebGL1Renderer } from '../../threejs/src/renderers/WebGL1Renderer.js';
export { ShaderLib } from '../../threejs/src/renderers/shaders/ShaderLib.js';
export { UniformsLib } from '../../threejs/src/renderers/shaders/UniformsLib.js';
export { UniformsUtils } from '../../threejs/src/renderers/shaders/UniformsUtils.js';
export { ShaderChunk } from '../../threejs/src/renderers/shaders/ShaderChunk.js';
export { FogExp2 } from '../../threejs/src/scenes/FogExp2.js';
export { Fog } from '../../threejs/src/scenes/Fog.js';
export { Scene } from '../../threejs/src/scenes/Scene.js';
export { Sprite } from '../../threejs/src/objects/Sprite.js';
export { LOD } from '../../threejs/src/objects/LOD.js';
export { SkinnedMesh } from '../../threejs/src/objects/SkinnedMesh.js';
export { Skeleton } from '../../threejs/src/objects/Skeleton.js';
export { Bone } from '../../threejs/src/objects/Bone.js';
export { Mesh } from '../../threejs/src/objects/Mesh.js';
export { InstancedMesh } from '../../threejs/src/objects/InstancedMesh.js';
export { LineSegments } from '../../threejs/src/objects/LineSegments.js';
export { LineLoop } from '../../threejs/src/objects/LineLoop.js';
export { Line } from '../../threejs/src/objects/Line.js';
export { Points } from '../../threejs/src/objects/Points.js';
export { Group } from '../../threejs/src/objects/Group.js';
export { VideoTexture } from '../../threejs/src/textures/VideoTexture.js';
export { DataTexture } from '../../threejs/src/textures/DataTexture.js';
export { DataTexture2DArray } from '../../threejs/src/textures/DataTexture2DArray.js';
export { DataTexture3D } from '../../threejs/src/textures/DataTexture3D.js';
export { CompressedTexture } from '../../threejs/src/textures/CompressedTexture.js';
export { CubeTexture } from '../../threejs/src/textures/CubeTexture.js';
export { CanvasTexture } from '../../threejs/src/textures/CanvasTexture.js';
export { DepthTexture } from '../../threejs/src/textures/DepthTexture.js';
export { Texture } from '../../threejs/src/textures/Texture.js';

export * from '../../threejs/src/geometries/Geometries.js';

// export { BoxGeometry } from '../../threejs/src/geometries/BoxGeometry.js';
//export * from './CircleGeometry.js';
//export * from './ConeGeometry.js';
//export { CylinderGeometry } from '../../threejs/src/geometries/CylinderGeometry.js';
//export * from './DodecahedronGeometry.js';
//export * from './EdgesGeometry.js';
//export { ExtrudeGeometry } from '../../threejs/src/geometries/ExtrudeGeometry.js';
//export * from './IcosahedronGeometry.js';
//export * from './LatheGeometry.js';
//export * from './OctahedronGeometry.js';
//export * from './PlaneGeometry.js';
//export * from './PolyhedronGeometry.js';
//export * from './RingGeometry.js';
//export * from './ShapeGeometry.js';
//export * from './SphereGeometry.js';
//export * from './TetrahedronGeometry.js';
//export { TorusGeometry } from '../../threejs/src/geometries/TorusGeometry.js';
//export * from './TorusKnotGeometry.js';
//export * from './TubeGeometry.js';
//export { WireframeGeometry } from '../../threejs/src/geometries/WireframeGeometry.js';

// export * from '../../threejs/src/materials/Materials.js';

export { ShadowMaterial } from '../../threejs/src/materials/ShadowMaterial.js';
export { SpriteMaterial } from '../../threejs/src/materials/SpriteMaterial.js';
export { RawShaderMaterial } from '../../threejs/src/materials/RawShaderMaterial.js';
export { ShaderMaterial } from '../../threejs/src/materials/ShaderMaterial.js';
export { PointsMaterial } from '../../threejs/src/materials/PointsMaterial.js';
export { MeshPhysicalMaterial } from '../../threejs/src/materials/MeshPhysicalMaterial.js';
export { MeshStandardMaterial } from '../../threejs/src/materials/MeshStandardMaterial.js';
export { MeshPhongMaterial } from '../../threejs/src/materials/MeshPhongMaterial.js';
export { MeshToonMaterial } from '../../threejs/src/materials/MeshToonMaterial.js';
export { MeshNormalMaterial } from '../../threejs/src/materials/MeshNormalMaterial.js';
export { MeshLambertMaterial } from '../../threejs/src/materials/MeshLambertMaterial.js';
export { MeshDepthMaterial } from '../../threejs/src/materials/MeshDepthMaterial.js';
export { MeshDistanceMaterial } from '../../threejs/src/materials/MeshDistanceMaterial.js';
export { MeshBasicMaterial } from '../../threejs/src/materials/MeshBasicMaterial.js';
export { MeshMatcapMaterial } from '../../threejs/src/materials/MeshMatcapMaterial.js';
export { LineDashedMaterial } from '../../threejs/src/materials/LineDashedMaterial.js';
export { LineBasicMaterial } from '../../threejs/src/materials/LineBasicMaterial.js';
export { Material } from '../../threejs/src/materials/Material.js';

export { AnimationLoader } from '../../threejs/src/loaders/AnimationLoader.js';
export { CompressedTextureLoader } from '../../threejs/src/loaders/CompressedTextureLoader.js';
export { CubeTextureLoader } from '../../threejs/src/loaders/CubeTextureLoader.js';
export { DataTextureLoader } from '../../threejs/src/loaders/DataTextureLoader.js';
export { TextureLoader } from '../../threejs/src/loaders/TextureLoader.js';
export { ObjectLoader } from '../../threejs/src/loaders/ObjectLoader.js';
export { MaterialLoader } from '../../threejs/src/loaders/MaterialLoader.js';
export { BufferGeometryLoader } from '../../threejs/src/loaders/BufferGeometryLoader.js';
export { DefaultLoadingManager, LoadingManager } from '../../threejs/src/loaders/LoadingManager.js';
export { ImageLoader } from '../../threejs/src/loaders/ImageLoader.js';
export { ImageBitmapLoader } from '../../threejs/src/loaders/ImageBitmapLoader.js';
export { FileLoader } from '../../threejs/src/loaders/FileLoader.js';
export { Loader } from '../../threejs/src/loaders/Loader.js';
export { LoaderUtils } from '../../threejs/src/loaders/LoaderUtils.js';
export { Cache } from '../../threejs/src/loaders/Cache.js';
export { AudioLoader } from '../../threejs/src/loaders/AudioLoader.js';
export { SpotLight } from '../../threejs/src/lights/SpotLight.js';
export { PointLight } from '../../threejs/src/lights/PointLight.js';
export { RectAreaLight } from '../../threejs/src/lights/RectAreaLight.js';
export { HemisphereLight } from '../../threejs/src/lights/HemisphereLight.js';
export { HemisphereLightProbe } from '../../threejs/src/lights/HemisphereLightProbe.js';
export { DirectionalLight } from '../../threejs/src/lights/DirectionalLight.js';
export { AmbientLight } from '../../threejs/src/lights/AmbientLight.js';
export { AmbientLightProbe } from '../../threejs/src/lights/AmbientLightProbe.js';
export { Light } from '../../threejs/src/lights/Light.js';
export { LightProbe } from '../../threejs/src/lights/LightProbe.js';
export { StereoCamera } from '../../threejs/src/cameras/StereoCamera.js';
export { PerspectiveCamera } from '../../threejs/src/cameras/PerspectiveCamera.js';
export { OrthographicCamera } from '../../threejs/src/cameras/OrthographicCamera.js';
export { CubeCamera } from '../../threejs/src/cameras/CubeCamera.js';
export { ArrayCamera } from '../../threejs/src/cameras/ArrayCamera.js';
export { Camera } from '../../threejs/src/cameras/Camera.js';
export { AudioListener } from '../../threejs/src/audio/AudioListener.js';
export { PositionalAudio } from '../../threejs/src/audio/PositionalAudio.js';
export { AudioContext } from '../../threejs/src/audio/AudioContext.js';
export { AudioAnalyser } from '../../threejs/src/audio/AudioAnalyser.js';
export { Audio } from '../../threejs/src/audio/Audio.js';
export { VectorKeyframeTrack } from '../../threejs/src/animation/tracks/VectorKeyframeTrack.js';
export { StringKeyframeTrack } from '../../threejs/src/animation/tracks/StringKeyframeTrack.js';
export { QuaternionKeyframeTrack } from '../../threejs/src/animation/tracks/QuaternionKeyframeTrack.js';
export { NumberKeyframeTrack } from '../../threejs/src/animation/tracks/NumberKeyframeTrack.js';
export { ColorKeyframeTrack } from '../../threejs/src/animation/tracks/ColorKeyframeTrack.js';
export { BooleanKeyframeTrack } from '../../threejs/src/animation/tracks/BooleanKeyframeTrack.js';
export { PropertyMixer } from '../../threejs/src/animation/PropertyMixer.js';
export { PropertyBinding } from '../../threejs/src/animation/PropertyBinding.js';
export { KeyframeTrack } from '../../threejs/src/animation/KeyframeTrack.js';
export { AnimationUtils } from '../../threejs/src/animation/AnimationUtils.js';
export { AnimationObjectGroup } from '../../threejs/src/animation/AnimationObjectGroup.js';
export { AnimationMixer } from '../../threejs/src/animation/AnimationMixer.js';
export { AnimationClip } from '../../threejs/src/animation/AnimationClip.js';
export { Uniform } from '../../threejs/src/core/Uniform.js';
export { InstancedBufferGeometry } from '../../threejs/src/core/InstancedBufferGeometry.js';
export { BufferGeometry } from '../../threejs/src/core/BufferGeometry.js';
export { InterleavedBufferAttribute } from '../../threejs/src/core/InterleavedBufferAttribute.js';
export { InstancedInterleavedBuffer } from '../../threejs/src/core/InstancedInterleavedBuffer.js';
export { InterleavedBuffer } from '../../threejs/src/core/InterleavedBuffer.js';
export { InstancedBufferAttribute } from '../../threejs/src/core/InstancedBufferAttribute.js';
export { GLBufferAttribute } from '../../threejs/src/core/GLBufferAttribute.js';
export * from '../../threejs/src/core/BufferAttribute.js';
export { Object3D } from '../../threejs/src/core/Object3D.js';
export { Raycaster } from '../../threejs/src/core/Raycaster.js';
export { Layers } from '../../threejs/src/core/Layers.js';
export { EventDispatcher } from '../../threejs/src/core/EventDispatcher.js';
export { Clock } from '../../threejs/src/core/Clock.js';
export { QuaternionLinearInterpolant } from '../../threejs/src/math/interpolants/QuaternionLinearInterpolant.js';
export { LinearInterpolant } from '../../threejs/src/math/interpolants/LinearInterpolant.js';
export { DiscreteInterpolant } from '../../threejs/src/math/interpolants/DiscreteInterpolant.js';
export { CubicInterpolant } from '../../threejs/src/math/interpolants/CubicInterpolant.js';
export { Interpolant } from '../../threejs/src/math/Interpolant.js';
export { Triangle } from '../../threejs/src/math/Triangle.js';
export * as MathUtils from '../../threejs/src/math/MathUtils.js';
export { Spherical } from '../../threejs/src/math/Spherical.js';
export { Cylindrical } from '../../threejs/src/math/Cylindrical.js';
export { Plane } from '../../threejs/src/math/Plane.js';
export { Frustum } from '../../threejs/src/math/Frustum.js';
export { Sphere } from '../../threejs/src/math/Sphere.js';
export { Ray } from '../../threejs/src/math/Ray.js';
export { Matrix4 } from '../../threejs/src/math/Matrix4.js';
export { Matrix3 } from '../../threejs/src/math/Matrix3.js';
export { Box3 } from '../../threejs/src/math/Box3.js';
export { Box2 } from '../../threejs/src/math/Box2.js';
export { Line3 } from '../../threejs/src/math/Line3.js';
export { Euler } from '../../threejs/src/math/Euler.js';
export { Vector4 } from '../../threejs/src/math/Vector4.js';
export { Vector3 } from '../../threejs/src/math/Vector3.js';
export { Vector2 } from '../../threejs/src/math/Vector2.js';
export { Quaternion } from '../../threejs/src/math/Quaternion.js';
export { Color } from '../../threejs/src/math/Color.js';
export { SphericalHarmonics3 } from '../../threejs/src/math/SphericalHarmonics3.js';
export { SpotLightHelper } from '../../threejs/src/helpers/SpotLightHelper.js';
export { SkeletonHelper } from '../../threejs/src/helpers/SkeletonHelper.js';
export { PointLightHelper } from '../../threejs/src/helpers/PointLightHelper.js';
export { HemisphereLightHelper } from '../../threejs/src/helpers/HemisphereLightHelper.js';
export { GridHelper } from '../../threejs/src/helpers/GridHelper.js';
export { PolarGridHelper } from '../../threejs/src/helpers/PolarGridHelper.js';
export { DirectionalLightHelper } from '../../threejs/src/helpers/DirectionalLightHelper.js';
export { CameraHelper } from '../../threejs/src/helpers/CameraHelper.js';
export { BoxHelper } from '../../threejs/src/helpers/BoxHelper.js';
export { Box3Helper } from '../../threejs/src/helpers/Box3Helper.js';
export { PlaneHelper } from '../../threejs/src/helpers/PlaneHelper.js';
export { ArrowHelper } from '../../threejs/src/helpers/ArrowHelper.js';
export { AxesHelper } from '../../threejs/src/helpers/AxesHelper.js';
export * from '../../threejs/src/extras/curves/Curves.js';
export { Shape } from '../../threejs/src/extras/core/Shape.js';
export { Path } from '../../threejs/src/extras/core/Path.js';
export { ShapePath } from '../../threejs/src/extras/core/ShapePath.js';
export { CurvePath } from '../../threejs/src/extras/core/CurvePath.js';
export { Curve } from '../../threejs/src/extras/core/Curve.js';
export { DataUtils } from '../../threejs/src/extras/DataUtils.js';
export { ImageUtils } from '../../threejs/src/extras/ImageUtils.js';
export { ShapeUtils } from '../../threejs/src/extras/ShapeUtils.js';
export { PMREMGenerator } from '../../threejs/src/extras/PMREMGenerator.js';
export { WebGLUtils } from '../../threejs/src/renderers/webgl/WebGLUtils.js';
export * from '../../threejs/src/constants.js';
export * from '../../threejs/src/Three.Legacy.js';

// jsroot part
export { TextGeometry } from '../../threejs/examples/jsm/geometries/TextGeometry.js';

import * as json1 from "../../threejs/examples/fonts/helvetiker_regular.typeface.json";

let HelveticerRegularFont = new Font(json1);

export { HelveticerRegularFont };

export { OrbitControls} from "../../threejs/examples/jsm/controls/OrbitControls.js";
export { TransformControls } from "../../threejs/examples/jsm/controls/TransformControls.js";
export { CopyShader } from "../../threejs/examples/jsm/shaders/CopyShader.js";
export { EffectComposer } from "../../threejs/examples/jsm/postprocessing/EffectComposer.js";
export { MaskPass } from "../../threejs/examples/jsm/postprocessing/MaskPass.js";
export { RenderPass } from "../../threejs/examples/jsm/postprocessing/RenderPass.js";
export { ShaderPass } from "../../threejs/examples/jsm/postprocessing/ShaderPass.js";
export { SSAOShader } from "../../threejs/examples/jsm/shaders/SSAOShader.js";
export { SSAOPass } from "../../threejs/examples/jsm/postprocessing/SSAOPass.js";
export { SimplexNoise } from "../../threejs/examples/jsm/math/SimplexNoise.js";
export { LuminosityHighPassShader } from "../../threejs/examples/jsm/shaders/LuminosityHighPassShader.js";
export { UnrealBloomPass } from "../../threejs/examples/jsm/postprocessing/UnrealBloomPass.js";

// end of jsroot part


if ( typeof __THREE_DEVTOOLS__ !== 'undefined' ) {

	/* eslint-disable no-undef */
	__THREE_DEVTOOLS__.dispatchEvent( new CustomEvent( 'register', { detail: {
		revision: REVISION,
	} } ) );
	/* eslint-enable no-undef */

}

if ( typeof window !== 'undefined' ) {

	if ( window.__THREE__ ) {

		console.warn( 'WARNING: Multiple instances of Three.js being imported.' );

	} else {

		window.__THREE__ = REVISION;

	}

}
