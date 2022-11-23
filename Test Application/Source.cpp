#include <Windows.h>
#include <Hauk.h>
#include "AppData.h"
#include "Sphere.h"
#include "Hitable_list.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include <float.h>


#define drand48() ((double)rand()/RAND_MAX)

vec3 random_in_unit_sphere();
bool bIsDrawn;
bool bIsThreadCreated;
vec3 color(const ray& r, hitable *world, int depth);
DWORD WINAPI RenderProc(LPVOID lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCommandLine, INT nCommandShow)
{
	cAnimatedWindow *AnimatedWindow = new(cAnimatedWindow);
	bIsDrawn = FALSE;
	bIsThreadCreated = false;
	AnimatedWindow->SetVideoMode(VideoDisplayModes[CUSTOM1]);

	AnimatedWindow->Animate();
	delete(AnimatedWindow);


	return 0;
}

LRESULT CALLBACK DrawProc(HWND hWindowHandle, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	if(bIsThreadCreated == false)
	{
		CreateThread(NULL, 0, RenderProc, (LPVOID)lParam, 0, NULL); 
		bIsThreadCreated = true;
	}

	return  0;

}

DWORD WINAPI RenderProc(LPVOID lParam)
{

	ANIMATED_WINDOW_INFO *sAnimateWindowInfo = (ANIMATED_WINDOW_INFO*)lParam;
	cDrawingBuffer *DrawBuffer = sAnimateWindowInfo->DrawingBufferObjectAddress;

	if (!bIsDrawn)
	{
		int nx = sAnimateWindowInfo->dwWindowLengthX;
		int ny = sAnimateWindowInfo->dwWindowHeightY;
		int ns = 100;
		
		hitable *list[2];
		//list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertain(vec3(0.8, 0.3, 0.3)));
		//list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertain(vec3(0.8, 0.8, 0.0)));
		list[0] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
		list[1] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
		
		hitable *world = new hitable_list(list, 2);
		camera cam;
		
		for (int j = ny - 1; j >= 0; j--)
		{
			for (int i = 0; i < nx; i++)
			{
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns; s++)
				{
					float u = float(i + drand48()) / float(nx);
					float v = float(j + drand48()) / float(ny);
					ray r = cam.get_ray(u, v);
					vec3 p = r.point_at_parameter(2.0);
					col += color(r, world, 0);
				}

				col /= float(ns);
				//col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);

				DrawBuffer->DrawPoint2D(i, j, RGB(ib, ig, ir));
			}
		}
		bIsDrawn = TRUE;
	}

	return 0;
}



vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation*color(scattered, world, depth+1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	} 
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}



