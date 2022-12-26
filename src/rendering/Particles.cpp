#include "Particles.h"

namespace vx
{
	
void Particles::draw(const Camera& camera, float dt) const
{
	glPointSize(1.0);

	/*m_compute_shader.activate();
	m_compute_shader.update("dt", dt);
	m_compute_shader.dispatch(m_vbo.size() / 32, 1, 1);*/

	m_shader.activate();
	m_shader.update("cameraMatrix", camera.viewMatrix());

	m_vao.bind();
	glDrawArrays(GL_POINTS, 0, m_vbo.size());
	m_vao.unbind();
}

} // namespace vx
