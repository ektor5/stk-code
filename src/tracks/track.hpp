//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Steve Baker <sjbaker1@airmail.net>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_TRACK_HPP
#define HEADER_TRACK_HPP

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  ifdef WIN32
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#  endif
#  include <GL/gl.h>
#endif
#include <plib/sg.h>
#include <plib/ssg.h>
#ifdef HAVE_IRRLICHT
#include "irrlicht.h"
using namespace irr;
#endif
#include <string>
#include <vector>
#include "LinearMath/btTransform.h"
#include "material.hpp"
#include "audio/music_information.hpp"
#include "utils/vec3.hpp"

class TriangleMesh;

class Track
{
private:
    float                    m_gravity;
    std::string              m_ident;
    std::string              m_screenshot;
    std::string              m_top_view;
    std::vector<MusicInformation*> m_music;
    std::vector<float>       m_start_x, m_start_y, m_start_z, m_start_heading;
    std::string              m_item_style;
    std::string              m_description;
    std::string              m_designer;
    std::string              m_filename;
    std::vector<std::string> m_groups;
#ifdef HAVE_IRRLICHT
    std::vector<scene::ISceneNode*> m_all_nodes;
    std::vector<scene::IMesh*>      m_all_meshes;
    scene::ILightSceneNode  *m_light;
#else
    ssgBranch               *m_model;
#endif
    TriangleMesh*            m_track_mesh;
    TriangleMesh*            m_non_collision_mesh;
    bool                     m_has_final_camera;
    Vec3                     m_camera_final_position;
    Vec3                     m_camera_final_hpr;
    bool                     m_is_arena;
    int                      m_version;
    
public:
    enum RoadSide{ RS_DONT_KNOW = -1, RS_LEFT = 0, RS_RIGHT = 1 };

    //An enum is not used for the QUAD_TRI_* constants because of limitations
    //of the conversion between enums and ints.
    static const int QUAD_TRI_NONE;
    static const int QUAD_TRI_FIRST;
    static const int QUAD_TRI_SECOND;

    static const int UNKNOWN_SECTOR;

    struct SegmentTriangle
    {
        int segment;
        int triangle;

        SegmentTriangle
        (
            int _segment,
            int _triangle
        ) : segment(_segment), triangle(_triangle) {};
    };

    std::string     m_name;
    bool            m_use_fog;
    float           m_fog_density;
    float           m_fog_start;
    float           m_fog_end;
#ifdef HAVE_IRRLICHT
    core::vector3df m_sun_position;
    video::SColorf  m_ambient_color;
    video::SColorf  m_specular_color;
    video::SColorf  m_diffuse_color;
    video::SColorf  m_sky_color;
    video::SColorf  m_fog_color;
#else
    sgVec4          m_sky_color;
    sgVec4          m_fog_color;
    sgVec3          m_sun_position;   /** Position of the sun */
    sgVec4          m_ambient_col;
    sgVec4          m_specular_col;
    sgVec4          m_diffuse_col;
#endif

    //FIXME: Maybe the next 4 vectors should be inside an struct and be used
    //from a vector of structs?
    //FIXME: should the driveline be set as a sgVec2?
    std::vector<Vec3>    m_driveline;
    /** Same as drivelines, but with stk_config->m_offroad_tolerance applied
     *  to widen the road (to make shortcut detection less severe). */
    std::vector<Vec3>    m_dl_with_tolerance_left;
    std::vector<Vec3>    m_dl_with_tolerance_right;
    std::vector<SGfloat> m_distance_from_start;
    std::vector<SGfloat> m_path_width;
    std::vector<SGfloat> m_angle;
    
    /** Start positions for arenas (unused in linear races) */
    std::vector<Vec3>   m_start_positions;
    
	//Left and Right drivelines for overhead map rendering.
	//(Should probably be private as they are only used internally right now)
    std::vector<Vec3> m_left_driveline;
    std::vector<Vec3> m_right_driveline;

    Vec3 m_driveline_min;
    Vec3 m_driveline_max;


    float m_total_distance;
    static const float NOHIT;

    float m_track_2d_width,  // Width and heigth of the 2d display of the track
          m_track_2d_height;
    float m_scale_x,        // Scaling to fit track into the size determined by
          m_scale_y;        // track2dWidth/Heightheigth
    bool m_do_stretch;      // 2d track display might be stretched to fit better

                       Track             (std::string filename,float w=100,
                                          float h=100, bool stretch=1);
                      ~Track             ();
    bool               isArena           () const { return m_is_arena; }
    void               cleanup           ();
    void               addDebugToScene   (int type                    ) const;
    void               draw2Dview        (float x_offset,
                                          float y_offset              ) const;
    void               drawScaled2D      (float x, float y, float w,
                                          float h                     ) const;

    void               findRoadSector    (const Vec3& XYZ, int *sector,
                                          bool with_tolerance=false) const;
    int                findOutOfRoadSector(const Vec3& XYZ,
                                           const RoadSide SIDE,
                                           const int CURR_SECTOR
                                           ) const;
    int                spatialToTrack    (Vec3& dst,
                                          const Vec3& POS,
                                          const int SECTOR            ) const;
    const Vec3&        trackToSpatial    (const int SECTOR) const;
    void               loadTrackModel    ();
    bool               isShortcut        (const int OLDSEC, const int NEWSEC) const;
    void               addMusic          (MusicInformation* mi)
                                                  {m_music.push_back(mi);       }
#ifdef HAVE_IRRLICHT
#else
    ssgBranch*         getModel          () const {return m_model;              }
#endif
    float              getGravity        () const {return m_gravity;            }
    /** Returns the version of the .track file. */
    int                getVersion        () const {return m_version;            }
    float              getTrackLength    () const {return m_total_distance;     }
    const std::string& getIdent          () const {return m_ident;              }
    const char*        getName           () const {return m_name.c_str();       }
    const std::vector<std::string>
                       getGroups         () const {return m_groups;             }
    void               startMusic        () const;
    const std::string& getFilename       () const {return m_filename;           }
#ifdef HAVE_IRRLICHT
    const core::vector3df& getSunPos     () const {return m_sun_position;       }
    const video::SColorf& getAmbientCol  () const {return m_ambient_color;      }
    const video::SColorf& getDiffuseCol  () const {return m_diffuse_color;      }
    const video::SColorf& getSpecularCol () const {return m_specular_color;     }
    const video::SColorf& getFogColor    () const {return m_fog_color;          }
    const video::SColorf& getSkyColor    () const {return m_sky_color;          }
#else
    const sgVec3& getSunPos              () const {return m_sun_position;       }
    const sgVec4& getAmbientCol          () const {return m_ambient_col;        }
    const sgVec4& getDiffuseCol          () const {return m_diffuse_col;        }
    const sgVec4& getSpecularCol         () const {return m_specular_col;       }
    const sgVec4& getFogColor            () const {return m_fog_color;          }
    const sgVec4& getSkyColor            () const {return m_sky_color;          }
#endif
    const bool&   useFog                 () const {return m_use_fog;            }
    const float&  getFogDensity          () const {return m_fog_density;        }
    const float&  getFogStart            () const {return m_fog_start;          }
    const float&  getFogEnd              () const {return m_fog_end;            }
    const std::string& getDescription    () const {return m_description;        }
    const std::string& getDesigner       () const {return m_designer;           }
    const std::string& getTopviewFile    () const {return m_top_view;           }
    const std::string& getScreenshotFile () const {return m_screenshot;         }
    const std::vector<SGfloat>& getWidth () const {return m_path_width;         }
    const std::string& getItemStyle   () const {return m_item_style;      }
    bool               hasFinalCamera    () const {return m_has_final_camera;   }
    const Vec3&        getCameraPosition () const {return m_camera_final_position;}
    const Vec3&        getCameraHPR      () const {return m_camera_final_hpr;   }
    btTransform        getStartTransform (unsigned int pos) const;
    void  getTerrainInfo(const Vec3 &pos, float *hot, Vec3* normal, 
                         const Material **material) const;
    void createPhysicsModel              ();
    void               glVtx             (sgVec2 v, float x_offset, float y_offset) const
    {
        glVertex2f(
            x_offset+(v[0]-m_driveline_min[0])*m_scale_x,
            y_offset+(v[1]-m_driveline_min[1])*m_scale_y);
    }

private:
    void  loadTrack                      (const std::string &filename);
    void  itemCommand                    (sgVec3 *xyz, int item_type, int bNeedHeight);
    void  loadDriveline                  ();
    void  readDrivelineFromFile          (std::vector<Vec3>& line,
                                         const std::string& file_ext);
#ifdef HAVE_IRRLICHT
    void  convertTrackToBullet           ();
#else
    void  convertTrackToBullet           (ssgEntity *track, sgMat4 m);
#endif
    float pointSideToLine(const Vec3& L1, const Vec3& L2,
                          const Vec3& P ) const;
    int   pointInQuad(const Vec3& A, const Vec3& B,
                      const Vec3& C, const Vec3& D, const Vec3& POINT ) const;
    void  getMusicInformation(std::vector<std::string>&             filenames, 
                              std::vector<MusicInformation*>& m_music   );
}
;   // class Track

#endif
