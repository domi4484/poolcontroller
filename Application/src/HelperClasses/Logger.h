/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:                                                         *
 * Created:      --.--.----                                        *
 *                                                                 *
********************************************************************/

#ifndef _LOGGER_H
#define _LOGGER_H

// Qt includes -----------------------------
#include <QObject>
#include <QString>

// Forward declarations --------------------
class QMutex;

//-----------------------------------------------------------------------------------------------------------------------------

/*!
 * \brief The Logger class provides methods to log messages of different severity levels.
 *
 * There is a log level property (see getLogLevel()/setLogLevel()) that defines the minimum severity level of a message to be actually logged.
 * The time when the message is issued is logged also.
 * \note All methods of this class are thread-safe.
 * \todo Reimplement using the database.
 */
class Logger : public QObject
{
  Q_OBJECT
public:

   /*!
    * \brief The LOG_LEVEL enum denotes the severity level of a log message.
    */
   enum LOG_LEVEL
   {
      LOG_ERROR    = -2,   //!< An (unrecoverable) error has occurred. User intervention is mandatory.
      LOG_WARNING  = -1,   //!< An error has occurred, which does not stop the device from operation. User intervention is optional.
      LOG_INFO     = 0,    //!< A normal operation event has occurred.
      LOG_TRACE    = 1,    //!< A detailed operation event has occurred.
      LOG_DEBUG    = 2,    //!< A more detailed operation event has occurred.
      LOG_VERBOSE  = 3     //!< A verbose operation (i.e. device command, SQL) has occurred.
   };

   explicit Logger(LOG_LEVEL logLevel,
                   QObject *parent = 0);
   virtual ~Logger();

   static void instanziate(LOG_LEVEL logLevel);
   static void destroy();
   static Logger *instance() { return s_Logger; }

   /*!
    * \brief Convert the given log level to a QString.
    * \param log_level The log level to convert.
    * \return A QString representing the given log level.
    */
   static QString toString (LOG_LEVEL log_level);

   /*!
    * \brief Log a message with LOG_ERROR level.
    * \param text The message to log.
    */
   static void error (const QString &text);

   /*!
    * \brief Log a message with LOG_WARNING level.
    * \param text The message to log.
    */
   static void warning (const QString &text);

   /*!
    * \brief Log a message with LOG_INFO level.
    * \param text The message to log.
    */
   static void info (const QString &text);

   /*!
    * \brief Log a message with LOG_TRACE level.
    * \param text The message to log.
    */
   static void trace (const QString &text);

   /*!
    * \brief Log a message with LOG_DEBUG level.
    * \param text The message to log.
    */
   static void debug (const QString &text);

   /*!
    * \brief Log a message with LOG_VERBOSE level.
    * \param text The message to log.
    */
   static void verbose (const QString &text);

private:

   static Logger *s_Logger;

   static QMutex *const s_Mutex;

   LOG_LEVEL     m_LogLevel;

   void write (LOG_LEVEL log_level, const QString &text);
};

#endif
