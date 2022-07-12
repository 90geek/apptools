/* Copyright (C) 2020 zhuyaliang https://github.com/zhuyaliang/
 *
 * This file is part of mate-recorder.
 *
 * mate-recorder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mate-recorder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mate-recorder.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <sys/sysinfo.h>
#include "loongson-server.h"
#include "config.h"

enum
{
    PROP_0,
    PROP_MANAGE_VERSION,
};

struct _LoongsonServerPrivate
{
    GDBusConnection *connection;
};

typedef GDBusMethodInvocation GDBusMth;
static void loongson_server_loongson_info_iface_init (LoongsonInfoIface *iface);

G_DEFINE_TYPE_WITH_CODE (LoongsonServer,loongson_server, LOONGSON_TYPE_INFO_SKELETON,
                         G_ADD_PRIVATE (LoongsonServer) G_IMPLEMENT_INTERFACE (
                         LOONGSON_TYPE_INFO, loongson_server_loongson_info_iface_init));

static void loongson_server_init (LoongsonServer *ls)
{
    ls->priv = loongson_server_get_instance_private (ls);
}

static void loongson_server_finalize (GObject *object)
{
    LoongsonServer *ls;

    ls = LOONGSON_SERVER (object);
}

static void get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    switch (prop_id)
    {
        case PROP_MANAGE_VERSION:
            g_value_set_string (value, VERSION);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
    switch (prop_id)
    {
        case PROP_MANAGE_VERSION:
            g_assert_not_reached ();
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void loongson_server_class_init (LoongsonServerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = loongson_server_finalize;
    object_class->get_property = get_property;
    object_class->set_property = set_property;

    g_object_class_override_property (object_class,
                                      PROP_MANAGE_VERSION,
                                      "daemon-version");
}

LoongsonServer *loongson_server_new(void)
{
    LoongsonServer *ls = NULL;

    ls = g_object_new (LOONGSON_TYPE_SERVER, NULL);

    return ls;
}

gboolean register_loongson_server (LoongsonServer *ls, GError **error)
{
    ls->priv->connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, error);
    if (ls->priv->connection == NULL)
    {
        return FALSE;
    }

    if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (ls),
                                           ls->priv->connection,
                                          "/org/loongson/info",
                                           error))
    {
        return FALSE;
    }

    return TRUE;
}

static const gchar *loongsoncast_get_daemon_version (LoongsonInfo *object)
{
    return VERSION;
}

static void loongson_server_loongson_admin_iface_init (LoongsonInfoIface *iface)
{
    iface->get_daemon_version =        loongsoncast_get_daemon_version;
}
