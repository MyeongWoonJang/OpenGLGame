#ifndef _title_scene
#define _title_scene

#include "game.h"
#include "TitleSpace.h"
#include "Light.h"

class TitleScene : public Scene
{
public:
	void update( const Time_t frame_time ) override
	{
		time += frame_time;
		sound::update();

		if ( time < 1000.f )
		{
			// [0, 2000] -> [0, 0.5]
			light.color += ( 0.45f / 1000 ) * frame_time;
		}

		light.update( frame_time );
		camera->update( frame_time );
		space.update();
	}

	void render() override
	{
		glClearColor( 0.f, 0.f, 0.f, 1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		const auto& campos = camera->coord_component_eye->getpivot();

		glUseProgram( shader->alpha_shader.id() );
		camera->set_shader_camera_transform( shader->alpha_view, shader->alpha_proj );
		glUniform3fv( shader->alpha_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->color_shader.id() );
		camera->set_shader_camera_transform( shader->color_view, shader->color_proj );
		glUniform3fv( shader->color_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->object_shader.id() );
		camera->set_shader_camera_transform( shader->object_view, shader->object_proj );
		glUniform3fv( shader->object_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->north_shader.id() );
		camera->set_shader_camera_transform( shader->north_view, shader->north_proj );

		light.pos = center->getpivot();

		light.render();
		space.render();

		glutSwapBuffers();
	}

	void reshape( int w, int h ) override
	{

	}

	void mouse( int button, int state, int x, int y ) override
	{

	}

	void motion( int x, int y ) override
	{

	}

	void passive_motion( int x, int y ) override
	{

	}

	void keyboard( unsigned char key, int x, int y ) override
	{
		static constexpr float step = 1.5f;

		switch ( key )
		{
		default:
			break;

		case 'w':
			center->movement -= glm::vec3( 0.f, 0.f, step );
			break;

		case 'a':
			center->movement -= glm::vec3( step, 0.f, 0.f );
			break;

		case 's':
			center->movement += glm::vec3( 0.f, 0.f, step );
			break;

		case 'd':
			center->movement += glm::vec3( step, 0.f, 0.f );
			break;

		case 'u':
			center->movement += glm::vec3( 0.f, step, 0.f );
			break;

		case 'j':
			center->movement -= glm::vec3( 0.f, step, 0.f );
			break;

		case 'y':
			center->rotation += glm::vec3( 0.f, 0.1f, 0.f );
			break;

		case 'Y':
			center->rotation -= glm::vec3( 0.f, 0.1f, 0.f );
			break;
		}
	}

	void keyboardup( unsigned char key, int x, int y ) override
	{

	}

	void special_keyboard( int key, int x, int y ) override
	{

	}

	TitleScene( SceneStatus& scene_status, const std::shared_ptr< GameShader >& shader = nullptr )
		: scene_status{ scene_status }, shader{ shader }, light{ shader }, space{ shader }
	{
		glEnable( GL_DEPTH_TEST );
		//glEnable( GL_CULL_FACE );

		camera.init();
		center.init();
		camera->coord_component_eye->adopt_base( center );
		camera->coord_component_at->adopt_base( center );
		//camera->coord_component_up->adopt_base( center );

		center->movement += glm::vec3{ 0.f, 10.f, -160.f };

		light.ambient = glm::vec3{ 0.4f, 0.4f, 0.4f };
		light.color = glm::vec3{ 0.5f, 0.5f, 0.5f };
		//light.pos = glm::vec3{ 0.f, 1000.f, -300.f };
		light.shininess = 4.f;

		s = sound::make( "resources/sound/logosound.wav", sound::mode::normal );
		s->play();

		camera->perspective();
	}

	~TitleScene()
	{
		glDisable( GL_DEPTH_TEST );
		//glDisable( GL_CULL_FACE );
	}

private:
	std::shared_ptr< GameShader > shader;
	SceneStatus& scene_status;
	CCamera camera;
	CCoord center;
	TitleSpace space;
	Region region;
	Light light;
	sound_ptr s;

	float time;
};

#endif