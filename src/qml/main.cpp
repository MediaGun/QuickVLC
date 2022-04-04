/******************************************************************************
* This file is part of QuickVLC - Qt and libvlc connection library.
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>, 
*               2022 Alexey Avramchik (OU Bamboo group) <aa@bam-boo.eu>
*
* QuickVLC is free software: you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published 
* by the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* QuickVLC is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty 
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with QuickVLC. If not, see <https://www.gnu.org/licenses/>.
******************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // this important so we can call makeCurrent from our rendering thread
    QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);

    QQmlApplicationEngine engine;
    const QUrl url(u"/home/alex_crack/_WORK/PROJECTS/Mediagun/quick-vlc/example/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
