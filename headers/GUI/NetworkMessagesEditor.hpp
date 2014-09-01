/*
 * Copyright: LaBRI / SCRIME / L'Arboretum
 *
 * Authors: Pascal Baltazar, Nicolas Hincker, Luc Vercellin and Myriam Desainte-Catherine (as of 16/03/2014)
 *
 * iscore.contact@gmail.com
 *
 * This software is an interactive intermedia sequencer.
 * It allows the precise and flexible scripting of interactive scenarios.
 * In contrast to most sequencers, i-score doesn’t produce any media, 
 * but controls other environments’ parameters, by creating snapshots 
 * and automations, and organizing them in time in a multi-linear way.
 * More about i-score on http://www.i-score.org
 *
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef NETWORK_MESSAGES_EDITOR_H
#define NETWORK_MESSAGES_EDITOR_H

/*!
 * \file NetworkMessagesEditor.hpp
 *
 * \author Luc Vercellin
 */

#include <QWidget>
#include <map>
#include <string>
#include <QStringList>
#include <QTableWidget>
#include <QTreeWidgetItem>

class QComboBox;
class QLineEdit;
class QVBoxLayout;
class QClipboard;
class QSpinBox;

/*!
 * \struct NetworkLine
 * \brief Used to store several composants of a network line.
 */
struct NetworkLine {
  QComboBox *devicesBox;
  QLineEdit *valueBox;
  QLineEdit *messageBox;
  unsigned int index;
};


/**!
 * \class NetworkMessagesEditor
 *
 * \brief Network messages editor, derived class from Qt's QWidget.
 */
class NetworkMessagesEditor : public QTableWidget {
  Q_OBJECT

  public:
    NetworkMessagesEditor(QWidget *parent);
    ~NetworkMessagesEditor();

    /*!
     * \brief Fills a list with all messages.
     *
     * return a list filled with messages
     */
    std::vector<std::string> computeMessages();

    /*!
     * \brief Adds a message to send with a specific device.
     *
     * \param device : the device to use
     * \param msg : the message to send
     */
    void addOneMessage(const std::string &device, const std::string &msg, const std::string &value);

    /*!
     * \brief Adds a list of messages.
     */
    void addMessages(const std::vector<std::string> &messages);

    /*!
     * \brief Clears messages contained in the editor.
     */

    /*!
     * \brief Set the list of messages.
     */
    void setMessages(const std::vector<std::string> &messages);

    void reset();

    static const int WIDTH = 350;     //!< Width of the editor

  public slots:
    /*!
     * \brief Push back an empty line.
     */
    void addLine();

    /*!
     * \brief Remove the selected lines if some.
     */
    void removeLines();

    void messageChanged();

    void valueChanged();

    void deviceChanged();

    /*!
     * \brief Called to paste messages from clipboard.
     */
    void importMessages();

    /*!
     * \brief Called to copy messages to clipboard.
     */
    void exportMessages();

    /*!
     * \brief Clears messages contained in the editor and set as modified.
     */
    void clear();

  signals:
    void messageChanged(const std::string &address);
    void messageRemoved(const std::string &address);
    void messagesChanged();

  private:
    std::string computeMessage(const NetworkLine &line);
    bool lineToStrings(const NetworkLine &line, std::string &device, std::string &message, std::string &value);
    void lineDeleted(const NetworkLine &line);

    void lineChanged(const NetworkLine &line, const std::string &changeString);


    QWidget *_parent;                       //!< The parent widget.
    unsigned int _currentLine;              //!< The next line index given.
    QVBoxLayout *_layout;                   //!< The layout handling lines.
    std::vector<NetworkLine> _networkLines; //!< Set of existing lines.
    std::map<QWidget*, unsigned int> _widgetIndex;
    static QStringList _devicesList;        //!< List of existing devices names.
    QClipboard *_clipboard;                 //!< Clipboard handling system's copy/paste.

  protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
#endif

