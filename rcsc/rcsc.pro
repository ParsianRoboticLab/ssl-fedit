TEMPLATE = lib
TARGET = rcsc
DESTDIR = ../lib
DEPENDPATH += . geom gz monitor param rcg
INCLUDEPATH += . .. ../boost ../zlib/include
#LIBS += -L../zlib -lz
LIBS += ../zlib/zlib1.dll
DEFINES += HAVE_LIBZ HAVE_WINDOWS_H
DEFINES += TRILIBRARY REDUCED CDT_ONLY NO_TIMER VOID=int REAL=double
CONFIG += staticlib warn_on release

# Input
HEADERS += factory.h \
           math_util.h \
           random.h \
           soccer_math.h \
           types.h \
           common/player_param.h \
           common/player_type.h \
           common/server_param.h \
           geom/triangle/triangle.h \
           geom/angle_deg.h \
           geom/circle_2d.h \
           geom/composite_region_2d.h \
           geom/delaunay_triangulation.h \
           geom/line_2d.h \
           geom/matrix_2d.h \
           geom/polygon_2d.h \
           geom/ray_2d.h \
           geom/rect_2d.h \
           geom/region_2d.h \
           geom/sector_2d.h \
           geom/segment_2d.h \
           geom/size_2d.h \
           geom/triangle_2d.h \
           geom/triangulation.h \
           geom/vector_2d.h \
           geom/voronoi_diagram.h \
           param/cmd_line_parser.h \
           param/conf_file_parser.h \
           param/param_map.h \
           param/param_parser.h \
           param/rcss_param_parser.h \
           rcg/handler.h \
           rcg/holder.h \
           rcg/reader.h \
           rcg/parser.h \
           rcg/parser_v1.h \
           rcg/parser_v2.h \
           rcg/parser_v3.h \
           rcg/parser_v4.h \
           rcg/parser_v5.h \
           ann/bpn1.h \
           ann/ngnet.h \
           ann/rbf.h \
           formation/formation.h \
           formation/formation_bpn.h \
           formation/formation_cdt.h \
           formation/formation_dt.h \
           formation/formation_knn.h \
           formation/formation_ngnet.h \
           formation/formation_rbf.h \
           formation/formation_sbsp.h \
           formation/formation_static.h \
           formation/formation_uva.h \
           formation/sample_data.h

SOURCES += common/player_param.cpp \
           common/player_type.cpp \
           common/server_param.cpp \
           geom/triangle/triangle.c \
           geom/angle_deg.cpp \
           geom/circle_2d.cpp \
           geom/composite_region_2d.cpp \
           geom/delaunay_triangulation.cpp \
           geom/line_2d.cpp \
           geom/matrix_2d.cpp \
           geom/polygon_2d.cpp \
           geom/ray_2d.cpp \
           geom/rect_2d.cpp \
           geom/sector_2d.cpp \
           geom/segment_2d.cpp \
           geom/triangle_2d.cpp \
           geom/triangulation.cpp \
           geom/vector_2d.cpp \
           geom/voronoi_diagram.cpp \
           param/cmd_line_parser.cpp \
           param/conf_file_parser.cpp \
           param/param_map.cpp \
           param/rcss_param_parser.cpp \
           rcg/util.cpp \
           ann/ngnet.cpp \
           ann/rbf.cpp \
           formation/formation.cpp \
           formation/formation_bpn.cpp \
           formation/formation_cdt.cpp \
           formation/formation_dt.cpp \
           formation/formation_knn.cpp \
           formation/formation_ngnet.cpp \
           formation/formation_rbf.cpp \
           formation/formation_sbsp.cpp \
           formation/formation_static.cpp \
           formation/formation_uva.cpp \
           formation/sample_data.cpp
