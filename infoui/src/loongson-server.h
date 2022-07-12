/*************************************************************************
  File Name: loongson-server.h

  Copyright (C) 2020  zhuyaliang https://github.com/zhuyaliang/
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Created Time: 2021年01月08日 星期五 14时29分13秒
 ************************************************************************/

#ifndef __LOONGSON_SERVER__
#define __LOONGSON_SERVER__

#include "loongson-generated.h"
G_BEGIN_DECLS

#define LOONGSON_TYPE_SERVER         (loongson_server_get_type ())
#define LOONGSON_SERVER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_SERVER, LoongsonServer))
#define LOONGSON_SERVER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), LOONGSON_TYPE_SERVER, LoongsonServerClass))
#define LOONGSON_IS_SERVER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_SERVER))
#define LOONGSON_IS_SERVER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), LOONGSON_TYPE_SERVER))
#define LOONGSON_SERVER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), LOONGSON_TYPE_SERVER, LoongsonServerClass))

typedef struct _LoongsonServer        LoongsonServer;
typedef struct _LoongsonServerClass   LoongsonServerClass;
typedef struct _LoongsonServerPrivate LoongsonServerPrivate;

struct _LoongsonServer
{
    LoongsonAdminSkeleton   parent_instance;
    LoongsonServerPrivate  *priv;
};

struct _LoongsonServerClass
{
    LoongsonAdminSkeletonClass parent_class;
};

GType             loongson_server_get_type                (void) G_GNUC_CONST;

LoongsonServer   *loongson_server_new                     (void);

gboolean          register_loongson_server                (LoongsonServer *ls,
                                                           GError        **error);

#endif
