// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.
#include "multipageloader.hh"
#include "tempfile.hh"
#include <QAtomicInt>
#include <QAuthenticator>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebFrame>
#include <QNetworkCookieJar>
#include <QFileInfo>
namespace wkhtmltopdf {

class MyNetworkAccessManager: public QNetworkAccessManager {
	Q_OBJECT
private:
	QSet<QString> allowed;
	const settings::LoadPage & settings;
public:
	void allow(QString path);
	MyNetworkAccessManager(const settings::LoadPage & s);
	QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
signals:
	void warning(const QString & text);
};

class MultiPageLoaderPrivate;
class ResourceObject;

class MyQWebPage: public QWebPage {
	Q_OBJECT ;
private:
	ResourceObject & resource;
public:
	MyQWebPage(ResourceObject & res);
	virtual void javaScriptAlert(QWebFrame * frame, const QString & msg);
	virtual bool javaScriptConfirm(QWebFrame * frame, const QString & msg);
	virtual bool javaScriptPrompt(QWebFrame * frame, const QString & msg, const QString & defaultValue, QString * result);
	virtual void javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID);
public slots:
	bool shouldInterruptJavaScript();
};

class ResourceObject: public QObject {
	Q_OBJECT
private:
	MyNetworkAccessManager networkAccessManager;
	QUrl url;
	int loginTry;
	int progress;
	bool finished;
	bool signalPrint;
	MultiPageLoaderPrivate & multiPageLoader;
public:
	ResourceObject(MultiPageLoaderPrivate & mpl, const QUrl & u, const settings::LoadPage & s);
	MyQWebPage webPage;
	LoaderObject lo;
	int httpErrorCode;
	const settings::LoadPage settings;	
public slots:
	void load();
	void loadStarted();
	void loadProgress(int progress);
	void loadFinished(bool ok);
	void printRequested(QWebFrame * frame);
	void loadDone();
	void handleAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
	void warning(const QString & str);
	void error(const QString & str);
	void sslErrors(QNetworkReply *reply, const QList<QSslError> &);
	void amfinished(QNetworkReply * reply);
};


class MyCookieJar: public QNetworkCookieJar {
private:
	QHash<QString, QList<QNetworkCookie> > extraCookies;
public:
	void useCookie(const QUrl & url, const QString & name, const QString & value);
	QList<QNetworkCookie> cookiesForUrl(const QUrl & url) const;
	void loadFromFile(const QString & path);
	void saveToFile(const QString & path);
};


class MultiPageLoaderPrivate: public QObject {
	Q_OBJECT
public:
	MyCookieJar * cookieJar;

	MultiPageLoader & outer;
	const settings::LoadGlobal settings;

	QList<ResourceObject *> resources;

	int loading;
	int progressSum;
	bool loadStartedEmitted;
	bool hasError;
	bool finishedEmitted;
	TempFile tempIn;

	MultiPageLoaderPrivate(const settings::LoadGlobal & settings, MultiPageLoader & o);
	~MultiPageLoaderPrivate(); 
	LoaderObject * addResource(const QUrl & url, const settings::LoadPage & settings);
	void load();
	void clearResources();
	void cancel();
public slots:
	void fail();
	void loadDone();
};

}
