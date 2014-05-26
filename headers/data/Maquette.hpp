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
#ifndef MAQUETTE_H
#define MAQUETTE_H

/*!
 * \file Maquette.h
 * \author Pascal Baltazar, Nicolas Hincker, Luc Vercellin and Myriam Desainte-Catherine 
 * \date March 14th 2014
 */

#include <QObject>
#include <QPoint>

#include <vector>
#include <map>
#include <string>
#include <list>
#include <utility>
#include <sstream>
#include "NetworkMessages.hpp"
#include "BasicBox.hpp"

#include "Engine.h"

//! Default network host.

#define NETWORK_LOCALHOST "127.0.0.1"

//! Default network port.
static const int NETWORK_PORT = 7000;
static const int OSC_NETWORK_PORT = 9999;

static const std::string PLAY_ENGINES_MESSAGE = "/Transport/Play";
static const std::string STOP_ENGINES_MESSAGE = "/Transport/Stop";
static const std::string PAUSE_ENGINES_MESSAGE = "/Transport/Pause";
static const std::string REWIND_ENGINES_MESSAGE = "/Transport/Rewind";
static const std::string STARTPOINT_ENGINES_MESSAGE = "/Transport/StartPoint";
static const std::string SPEED_ENGINES_MESSAGE = "/Transport/Speed";
static const std::string NEXT_TRIGGER_MESSAGE = "/Transport/Next";

#define NETWORK_PORT_STR "7000"

class PaletteActor;

class BasicBox;
class ParentBox;
class BoundingBox;
class MaquetteScene;
class QDomDocument;
class Engines;
class AbstractParentBox;
class AbstractRelation;
class Relation;
class TriggerPoint;
class QApplication;

//! Enum containing various error messages.
typedef enum { SUCCESS = 1, NO_MODIFICATION = 0, RETURN_ERROR = -1,
               ARGS_ERROR = -2 } ErrorMessage;

//! Enum containing various warning levels.

typedef enum { INDICATION_LEVEL, WARNING_LEVEL, ERROR_LEVEL } ErrorLevel;

/// \todo les objets graphiques Qt maintiennent eux-même leurs tailles et positions. (par jaime Chao)
/*!
 * \brief Structure used to contain boxes position or size.
 * Used for interaction with MaquetteScene.
 */
struct Coords {
  float topLeftX;
  float topLeftY;
  float sizeX;
  float sizeY;
};

/*!
 * \class MyDevice
 *
 * \brief Network device handling a name, a plug-in, a port and a network host.
 */
class MyDevice {
  public:
    MyDevice(const std::string &nameArg = "NO_STR", const std::string &pluginArg = "NO_STR",
             unsigned int networkPortArg = NETWORK_PORT, const std::string &networkHostArg = NETWORK_LOCALHOST)
      : name(nameArg), plugin(pluginArg), networkPort(networkPortArg), networkHost(networkHostArg)
    {
    }
    MyDevice(const MyDevice &other)
    {
      name = other.name;
      plugin = other.plugin;
      networkPort = other.networkPort;
      networkHost = other.networkHost;
    }
    ~MyDevice()
    {
    }
    std::string name;         //!< Name of the device.
    std::string plugin;       //!< plugin used by the device.
    unsigned int networkPort; //!< Network port used by device.
    std::string networkHost;  //!< Network host used by device.
};

/*!
 * \class Maquette
 *
 * \brief Representation of a musical piece.
 *
 * A musical piece will be represented by this class. It's composed of a set
 * of boxes organised on a a timeline and possibly with temporal constraints.
 * A Maquette is associated with a MaquetteScene that manages graphical
 * representation and user interaction.
 *
 */

class Maquette : public QObject
{
  Q_OBJECT

  public:

    /// \todo Quel est le rôle de getInstance() avec instanciantion en static ? Faire de Maquette une classe statique ! Est-ce la bonne manière ou plutôt mettre dans un namespace. (par jaime Chao)
    static Maquette * getInstance()
    {
      static Maquette *instance = new Maquette();
      return instance;
    }

    ~Maquette();

    /*!
     * \brief Initialise the maquette elements.
     */
    void init(); /// \todo Une méthode init() ne devrait pas être utilisée. surtout en public !!! Elle peut laisser l'invariant de classe instable à tout moment. (par jaime Chao)

    /*!
     * \brief Sets a new scene.
     *
     * \param scene : the new scene to be set
     */
    void setScene(MaquetteScene *scene);

    /*!
     * \brief Evaluates the global duration of the maquette.
     *
     * \return the duration of the maquette
     */
    int duration();

    /*!
     * \brief Gets a set of relations IDs involving a particular entity.
     *
     * \param entityID : the entity to get relations from
     * \return a vector of relations IDs involving entityID
     */
    std::vector<unsigned int> getRelationsIDs(unsigned int entityID);

    /*!
     * \brief Gets the current network device used.
     *
     * \return the current network device used
     */
    std::string getNetworkDevice();
    void addNetworkDevice(string deviceName, string plugin, string ip, unsigned int destinationPort, unsigned int receptionPort = 0);

    /*!
     * \brief Gets a set of the available network devices.
     *
     * \return a set of the available network devices
     */
    std::map<std::string, MyDevice> getNetworkDevices();

    /*!
     * \brief Gets the current network port used.
     *
     * \return the current network port used
     */
    std::string getListeningPort();

    /*!
     * \brief Gets a set of the available listening ports.
     *
     * \return a set of the available listening ports
     */
    std::vector<std::string> getListeningPorts();

    /*!
     * \brief Gets the current network IP used.
     *
     * \return the current network IP used
     */
    std::string getNetworkHost();

    /*!
     * \brief Updates a specific network device with new parameters.
     *
     * \param deviceName : the device to be set
     * \param pluginName : the new plugin to use
     * \param IP : the new IP to use
     * \param port : the new port to use
     */
    void changeNetworkDevice(const std::string &deviceName, const std::string &pluginName, const std::string &IP, const unsigned int &port);

    /*!
     * \brief Gets the set of devices and their respective requestability.
     *
     * \param deviceName : the devices to be filled
     */
    void getNetworkDeviceNames(std::vector<std::string>& deviceName);

    bool isNetworkDeviceRequestable(string deviceName);
    
    /*!
     * \brief Requests a snapshot to the Engines.
     *
     * \param address : the address to be snapped.
     * \return the snapshot taken
     */
    std::vector<std::string> requestNetworkSnapShot(const std::string &address);

    /*!
     * \brief Adds a curve at specified address.
     *
     * \param boxID : the box to add a curve to
     * \param address : the address concerned by the curve
     */
    void addCurve(unsigned int boxID, const std::string &address);

    /*!
     * \brief Removes a curve at specified address.
     *
     * \param boxID : the box to remove a curve from
     * \param address : the address concerned by the curve
     */
    void removeCurve(unsigned int boxID, const std::string &address);

    /*!
     * \brief Removes all curves for a specified box.
     *
     * \param boxID : the box to remove curves from
     */
    void clearCurves(unsigned int boxID);

    /*!
     * \brief Gets the addresses associated to curves for a specified box.
     *
     * \param boxID : the box to get curves from
     */
    std::vector<std::string> getCurvesAddresses(unsigned int boxID);

    void setCurveRedundancy(unsigned int boxID, const std::string &address, bool redundancy);
    bool getCurveRedundancy(unsigned int boxID, const std::string &address);

    void setCurveSampleRate(unsigned int boxID, const std::string &address, int sampleRate);
    unsigned int getCurveSampleRate(unsigned int boxID, const std::string &address);

    void setCurveMuteState(unsigned int boxID, const std::string &address, bool muteState);
    bool getCurveMuteState(unsigned int boxID, const std::string &address);


    /*!
     * \brief Sets sections of a curve at a specific address in a box.
     *
     * \param boxID : the ID of the box
     * \param address : the address of the curve
     * \param argPosition : the index of the curve
     * \param xPercents : x-axis values in percents (0 < % < 100)
     * \param yValues : y-axis values
     * \param sectionType : types of curve's subsections (curves...)
     * \param coeff : coeff of curve's subsections' types (for CURVES_POW: 0 to 0.9999 => sqrt, 1 => linear, 1 to infinity => pow)
     *
     * \return if curves were set correctly
     */
    bool setCurveSections(unsigned int boxID, const std::string &address, unsigned int argPosition,
                          const std::vector<float> &xPercents, const std::vector<float> &yValues, const std::vector<short> &sectionType, const std::vector<float> &coeff);

    /*!
     * \brief Gets attributes of a curve at a specific address in a box.
     *
     * \param boxID : the ID of the box
     * \param address : the address of the curve
     * \param argPosition : the index of the curve
     * \param sampleRate : the sample rate to be set
     * \param redundancy : the redundancy to be set
     * \param values : the breakpoints' values to be set for y-axis
     * \param argTypes : the respective types of the values to be set
     * \param xPercents : the values to be set for x-axis in percents (0 < % < 100)
     * \param yValues : y-axis values to be set
     * \param sectionType : types of curve's subsections (curves...)
     * \param coeff : coeff of curve's subsections' types to be set (for CURVES_POW: 0 to 0.9999 => sqrt, 1 => linear, 1 to infinity => pow)
     *
     * \return if curves were set correctly
     */
    bool getCurveAttributes(unsigned int boxID, const std::string &address, unsigned int argPosition, unsigned int & sampleRate,
                            bool &redundancy, bool &interpolate, std::vector<float>& values, std::vector<std::string> & argTypes, std::vector<float> &xPercents,
                            std::vector<float> &yValues, std::vector<short> &sectionType, std::vector<float> &coeff);

    bool getCurveValues(unsigned int boxID, const std::string &address, unsigned int argPosition, std::vector<float> &values);

    /*!
     * \brief Raised when execution is finished
     */
    void executionFinished();

    /*!
     * \brief Gets the set of messages to send for the beginning of a box.
     *
     * \param boxID : the box to get messages set from
     *
     * \return a set of the messages to send for the beginning of a box
     */
    std::vector<std::string> firstMessagesToSend(unsigned int boxID);

    /*!
     * \brief Gets the set of messages to send for the end of a box.
     *
     * \param boxID : the box to get messages set from
     *
     * \return a set of the messages to send for the end of a box
     */
    std::vector<std::string> lastMessagesToSend(unsigned int boxID);

    /*!
     * \brief Sets the set of messages to send for the end of a box.
     *
     * \param boxID : the box to get messages set from
     * \param messages : the new set of the messages
     *
     * \return if messages could be set
     */
    bool setStartMessagesToSend(unsigned int boxID, NetworkMessages *messages, bool sort = true);
    NetworkMessages *startMessages(unsigned int boxID);

    /*!
     * \brief Sets the set of treeItems to send for the beginning of a box.
     *
     * \param boxID : the box to get treeItems set from
     * \param itemsSelected : the set of the items selected in the networkTree
     *
     * \return if itemsSelected could be set
     */
    bool setSelectedItemsToSend(unsigned int boxID, QMap<QTreeWidgetItem*, Data> itemsSelected);

    /*!
     * \brief Calls the NetworkTree method getAbsoluteAddress.
     */
    QString getAbsoluteAddress(QTreeWidgetItem *item);

    /*!
     * \brief Sorts messages by priority than alphabetical.
     *
     * \param messages : message to sort
     *
     * \return if itemsSelected could be set
     */
    std::vector<std::string> sortByPriority(NetworkMessages *messages);
    static int compareByPriority(const QPair<QTreeWidgetItem *, std::string> v1, const QPair<QTreeWidgetItem *, std::string> v2);


    /*!
     * \brief Sets the set of treeItems expanded.
     *
     * \param boxID : the box to get treeItems set from
     * \param itemsExpanded : the set of the items expanded in the networkTree
     *
     * \return if itemsExtanded could be set
     */
    bool setExpandedItemsList(unsigned int boxID, QList<QTreeWidgetItem*> itemsExpanded);

    /*!
     * \brief Add to treeItems expanded list.
     *
     * \param boxID : the box to get treeItems set from
     * \param item : the items to be add
     *
     * \return if itemsExtanded could be add
     */
    bool addToExpandedItemsList(unsigned int boxID, QTreeWidgetItem* item);

    /*!
     * \brief Sets start messages.
     *
     * \param boxID : the box to get treeItems set from
     * \param item : the NetworkMessages to be set
     *
     * \return if networkMessages could be set
     */
    bool setStartMessages(unsigned int boxID, NetworkMessages* nm);

    /*!
     * \brief Sets end messages.
     *
     * \param boxID : the box to get treeItems set from
     * \param item : the NetworkMessages to be set
     *
     * \return if networkMessages could be set
     */
    bool setEndMessages(unsigned int boxID, NetworkMessages* nm);
    NetworkMessages *endMessages(unsigned int boxID);

    /*!
     * \brief Remove from treeItems expanded list.
     *
     * \param boxID : the box to get treeItems set from
     * \param item : the items to be removed
     *
     * \return if itemsExtanded could be removed
     */
    bool removeFromExpandedItemsList(unsigned int boxID, QTreeWidgetItem* item);

    /*!
     * \brief Sets the set of messages to send for the end of a box.
     *
     * \param boxID : the box to get messages set from
     * \param messages : the new set of the messages
     *
     * \return if messages could be set
     */
    bool setEndMessagesToSend(unsigned int boxID, NetworkMessages *messages, bool sort = true);

    /*!
     * \brief Sends a specific message with current device.
     *
     * \param message : the message to send
     * \return true if message could be sent
     */
    bool sendMessage(const std::string &message);

    /*!
     * \brief Adds a parent box to the maquette.
     *
     * \param corner1 : a first corner of the box's bounding rectangle
     * \param corner2 : the second corner of the box's bounding rectangle
     * \param name : the name given to the box
     * \param mother : the mother of the box
     * \return the ID of created parent box
     */
    unsigned int addParentBox(const QPointF & corner1, const QPointF & corner2, const std::string & name, unsigned int mother);

    /*!
     * \brief Adds a new parent box to the maquette with specific information.
     *
     * \param abstract : the abstract parent box containing information about the box
     * \return the ID of box created
     */
    unsigned int addParentBox(const AbstractParentBox &abstract);

    /*!
     * \brief Gets the whole set of parent boxes.
     *
     * \return the whole set of parent boxes
     */
    std::map<unsigned int, ParentBox*> parentBoxes();

    /*!
     * \brief Adds a new AntPost relation between 2 objects extremities.
     *
     * \param ID1 : the first box in the relation
     * \param firstExtremum : the extremity of the first box used for relation
     * \param ID2 : the second box in the relation
     * \param secondExtremum : the extremity of the second box used for relation
     * \param antPostType : the anterior/posterior relation type
     * \return a specific error message in ErrorMessage enum
     */
    int addRelation(unsigned int ID1, BoxExtremity firstExtremum, unsigned int ID2,
                    BoxExtremity secondExtremum, int antPostType);

    /*!
     * \brief Adds a new AntPost relation described by a AbstractRelation.
     *
     * \param abstract : the information about the relation
     * \return a specific error message in ErrorMessage enum
     */
    int addRelation(const AbstractRelation &abstract);

    /*!
     * \brief Change boundaries for a specific relation.
     *
     * \param relID : the ID of the relation to change boundaries from
     * \param minBound : the minimal boundary to be set
     * \param maxBound : the maximal boundary to be set
     */
    void changeRelationBounds(unsigned int relID, const float &minBound, const float &maxBound);

    /*!
     * \brief Creates a new interval relation between 2 objects.
     *
     * \param ID1 : the first box ID in the interval
     * \param ID2 : the second box ID in the interval
     * \param value : the interval value
     * \param tolerance : the interval tolerance
     * \return true if the interval could be created
     */
    bool addInterval(unsigned int ID1, unsigned int ID2, int value, int tolerance);

    /*!
     * \brief Removes a temporal relation.
     * \param relationID : the relation to remove
     */
    void removeRelation(unsigned int relationID);

    /*!
     * \brief Determines if a relation exists between two boxes.
     *
     * \param ID1 : the first box in possible relation
     * \param ID2 : the first box in possible relation
     * \return if boxes are related
     */
    bool areRelated(unsigned int ID1, unsigned int ID2);

    /*!
     * \brief Adds a new trigger point and link it to a control point.
     *
     * \param boxID : the box concerned by the trigger point
     * \param extremity : the extremity of the box to link to trigger point
     * \param message : the message to link with the trigger point
     *
     * \return a specific error message in ErrorMessage enum
     */
    int addTriggerPoint(unsigned int boxID, BoxExtremity extremity, const std::string &message);
    int addTriggerPoint(const AbstractTriggerPoint &abstract);

    /*!
     * \brief Removes a trigger point.
     *
     * \param trgID : the trigger point ID
     */
    void removeTriggerPoint(unsigned int trgID);

    /*!
     * \brief Gets a trigger point.
     *
     * \param trgID : the trigger point ID
     *
     * \return the trigger point
     */
    TriggerPoint* getTriggerPoint(unsigned int trgID);

    /*!
     * \brief Trigger a trigger point.
     *
     * \param triggerPoint : the trigger point to trigger
     */
    void trigger(TriggerPoint *triggerPoint);

    /*!
     * \brief Set trigger point 's message.
     *
     * \param trgID : the trigger point to be modified
     * \param message : the new message
     */
    bool setTriggerPointMessage(unsigned int trgID, const std::string &message);

    /*!
     * \brief Create a time condition between boxes.
     *
     * \param boxes : the boxes to condition.
     */
    unsigned int createCondition(QList<BasicBox *> boxes);

    /*!
     * Add a box to the condition.
     *
     * \param conditionId : the ID of the condition
     * \param box : the box to add
     */
    void attachToCondition(unsigned int conditionId, BasicBox *box);

    /*!
     * Dettach a box from the condition.
     *
     * \param conditionId : the ID of the condition
     * \param box : the box to detach
     */
    void detachFromCondition(unsigned int conditionId, BasicBox *box);

    void setConditionMessage(unsigned int conditionId, std::string disposeMessage);
    std::string getConditionMessage(TimeConditionId conditionId);

    /*!
     * Delete the specified TimeCondition.
     *
     * \param conditionId : the ID of the condition to delete
     */
    void deleteCondition(TimeConditionId conditionId);

    void getConditionsId(std::vector<unsigned int> &conditionsId);

    void getBoxesIdFromCondition(TimeConditionId conditionId, std::vector<unsigned int> &boxesId);


    /*!
     * \brief Perform moving or resizing for a set of boxes.
     *
     * \param boxes : the boxes that have to be transformed with their new coordinates
     * \return false if the Engines can not perform the transformation
     */
    bool updateBoxes(const std::map<unsigned int, Coords> &boxes);

    /*!
     * \brief Perform moving or resizing for a single box.
     *
     * \param box : the box that has to be transformed with its new coordinates
     * \return false if the Engines can not perform the transformation
     */
    bool updateBox(unsigned int boxID, const Coords &coord);

    /*!
     * \brief Informs relations that boxes have been moved.
     */
    void updateRelations();

    /*!
     * \brief Clears the maquette.
     */
    void clear();

    /*!
     * \brief Remove a box from the maquette.
     *
     * \param boxID : the box to remove
     * \return removed temporal relations
     */
    std::vector<unsigned int> removeBox(unsigned int boxID);

    /*!
     * \brief Gets a specific box.
     *
     * \param : the ID of the box wanted
     * \return the box specified by the ID
     */
    BasicBox* getBox(unsigned int ID);
    
    /*!
     * \brief Set the color of a specific box.
     *
     * \param ID : the ID of the box wanted
     * \param newColor : the box specified by the ID
     */
    void setBoxColor(unsigned int ID, QColor newColor);

    /*!
     * \brief Set the name of a specific box.
     *
     * \param ID : the ID of the box wanted
     * \param name : the box specified by the ID
     */
    void setBoxName(unsigned int ID, std::string name);

    /*!
     * \brief Gets a specific relation.
     *
     * \param : the ID of the relation wanted
     * \return the relation specified by the ID
     */
    Relation* getRelation(unsigned int ID);

    /*!
     * \brief Returns the next sequential name number.
     *
     * \return the next ID to be used for box creation
     */
    unsigned int sequentialID();

    /*!
     * \brief Saves the current composition into a file.
     *
     * \param fileName : the file to save current composition into
     */
    void save(const std::string &fileName);

    /*!
     * \brief Loads a file into a new composition.
     *
     * \param fileName : the file to load composition from
     */
    void load(const std::string &fileName);

    /*!
     * \brief Gets the current execution time in ms.
     *
     * \return the current execution time in ms
     */
    unsigned int getCurrentTime() const;

    /*!
     * \brief Gets the box progress ratio.
     *
     * \return the box progress ratio
     */
    float getProgression(unsigned int boxID);

    /*!
     * \brief Requests a snapshot of the network on a namespace.
     *
     * \param address : the address to take snapshot on
     * \param nodeType : the object type under at the address
     * \param nodes : the nodes to be filled
     * \param leaves : the leaves to be filled
     * \param attributes : the attributes to be filled
     * \param attributesValue : the respective values of the attributes to be filled
     */
    int requestNetworkNamespace(const std::string &address, std::string &nodeType, std::vector<std::string>& nodes, std::vector<std::string>& leaves,
                                std::vector<std::string>& attributes, std::vector<std::string>& attributesValue);
    /*!
     * \brief Refresh the network's namespace.
     */
    void refreshNetworkNamespace(const std::string &application);

    /*!
     * \brief Requests a snapshot of the network on a namespace.
     *
     * \param address : the address to take snapshot on
     * \param nodeType : the object type under at the address
     * \param nodes : the nodes to be filled
     * \param leaves : the leaves to be filled
     * \param attributes : the attributes to be filled
     * \param attributesValue : the respective values of the attributes to be filled
     */
    int requestObjectAttribruteValue(const std::string &address, const std::string &attributeName, std::vector<std::string>& value);    

    void setRangeBoundMin(unsigned int boxID, const string &address, float value);
    void setRangeBoundMax(unsigned int boxID, const string &address, float value);

    /*!
     * \brief Update boxes from Engines.
     */
    void updateBoxesFromEngines();
    inline MaquetteScene *
    scene(){ return _scene; }
    static const unsigned int SIZE;

    bool getDeviceLocalHost(std::string deviceName, std::string protocol, std::string &localHost);
    bool getDeviceLocalHost(std::string deviceName, std::string &localHost);

    bool getDevicePort(std::string deviceName,  std::string protocol, unsigned int &port);
    bool getDevicePort(std::string deviceName, unsigned int &port);
    bool getDevicePorts(std::string deviceName, std::string protocol, std::vector<int> &portVector);

    int getOSCInputPort();
    int getMinuitInputPort();

    bool getDeviceProtocol(std::string deviceName, std::string &protocol);
    std::vector<std::string> getProtocolsName();

    bool setDeviceName(std::string device, std::string newName);
    bool setDevicePort(std::string device, int destinationPort, int receptionPort = 0);
    bool setDeviceLocalHost(std::string device, std::string localHost);
    bool setDeviceProtocol(std::string device, std::string protocol);

    bool loadNetworkNamespace(const string &application, const string &filepath);

  public slots:
    
    /*!
     * \brief Sets the time offset value in ms where the engine will start from at the nex execution. The boolean "mute" mutes or not the dump of all messages (the scene state at timeOffset).
     */
    void setTimeOffset(unsigned int timeOffset, bool mute = NO);
    
    unsigned int getTimeOffset();

    /*!
     * \brief Turn engine execution on depending on the context
     */
    void turnExecutionOn();
    
    /*!
     * \brief Is the engine running ?
     *
     * \return true if the engine is running
     *
     */
    bool isExecutionOn();

    /*!
     * \brief Turn engine execution off depending on the context
     */
    void turnExecutionOff();
    void stopPlayingAndGoToStart();
    void stopPlayingAndGoToTimeOffset(unsigned int timeOffset);
    void stopPlayingAndGoToCurrentTime();
    
    
    void pauseExecution();
    void resumeExecution();
    
    bool isExecutionPaused();

    /*!
     * \brief Sets a new acceleration factor.
     *
     * \param factor : the new acceleration factor value between 0 and 1
     */
    void setAccelerationFactor(const float &factor);
    double accelerationFactor();
    
    /*!
     * \brief Sets a new zoom factor into the engine
     *
     * \param zoom : the new zoom factor value
     */
    void setViewZoom(const QPointF zoom);
    
    /*!
     * \brief Sets a new center position into the engine
     *
     * \param zoom : the new zoom factor value
     */
    void setViewPosition(const QPointF position);

    /*!
     * \brief Called by the callback when the running state of a box change
     *
     * \param boxID : the box whose running state have cahnged
     * \param runnging : the new running state
     */
    void updateBoxRunningStatus(unsigned int boxID, bool running);

    /*!
     * \brief Called by the callback when the active state of a triggerPoint change.
     *
     * \param waiting : new active state of the triggerPoint
     * \param trgID : trigger point ID
     */
    void updateTriggerPointActiveStatus(unsigned int trgID, bool active);
    
    inline std::map<unsigned int, BasicBox*> getBoxes(){ return _boxes; }
    inline QList<BasicBox *> getRecordingBoxes(){return _recordingBoxes ;}
    /*!
     * \brief When a time offset is entered, the scenario before this value is simulated.
     * Messages (final state of each boxes) are sended to the engine.
     */
    void initSceneState();
    void setStartMessageToSend(unsigned int boxID, QTreeWidgetItem *item, QString address);
    void setEndMessageToSend(unsigned int boxID, QTreeWidgetItem *item, QString address);
    std::vector<std::string> getPlugins();
    void removeNetworkDevice(string deviceName);

    /*!
     * \brief Requests the rangeBounds of an object to the Engines.
     *
     * \param address : the address to be snapped.
     * \param rangeBounds : Result to be field.
     */
    int getRangeBounds(const std::string& address, std::vector<float>& rangeBounds);    

    /*!
     * Gets the type of an object.
     *
     * \param address : the object's address. ex : /deviceName/address1/address2/
     * \param nodeType : will be filled with the type.
     *
     * \return True(1) or false(0) if the request failed or not.
     */
    int getObjectType(const std::string & address, std::string & nodeType);

    /*!
     * Gets the priority of an object.
     *
     * \param address : the object's address. ex : /deviceName/address1/address2/
     * \param priority : will be filled with the priority.
     *
     * \return True(1) or false(0) if the request failed or not.
     */
    int getPriority(const std::string & address, unsigned int & priority);

    /*!
     * Gets the children nodes of an object.
     *
     * \param address : the object's address. ex : /deviceName/address1/address2/
     * \param children : will be filled with the children' addresses.
     *
     * \return True(1) or false(0) if the request failed or not.
     */
    int getObjectChildren(const std::string & address, std::vector<std::string>& children);

    /*!
     * \brief Updates all boxes' attributes
     */
    void updateBoxesAttributes();

    /*!
     * \brief Sets address record mode.
     */
    void setCurveRecording(unsigned int boxID, std::string address, bool activated);

  private:
    /*!
     * \brief Generates the triggerQueueList.
     */
    void generateTriggerQueue();

    Maquette();

    /*!
     * \brief Adds a box existing in Engines to the maquette with given ID.
     *
     * \param ID : the ID to assign to the box
     * \param corner1 : a first corner of the box's bounding rectangle
     * \param corner2 : the second corner of the box's bounding rectangle
     * \param name : the name given to the box
     * \param mother : the mother to assign to the box
     * \return the ID of created parent box
     */
    unsigned int addParentBox(unsigned int ID, const QPointF & corner1, const QPointF & corner2, const std::string &name,
                              unsigned int mother);
    unsigned int addParentBox(unsigned int ID, const unsigned int date, const unsigned int topLeftY, const unsigned int sizeY, const unsigned int duration, const std::string &name,
                              unsigned int mother, QColor color);

    string extractAddress(string msg);
    string extractValue(string msg);

    /*!
     * \brief Saves a box into doc.
     *
     * \param ID : the ID of the box to be saved
     */
    void saveBox(unsigned int ID);

    /*!
     * \brief Update curves for a box by specifying star end end messages.
     *
     * \param startMsgs : the start messages of the box
     * \param endMsgs : the end messages of the box
     */
    void updateCurves(unsigned int boxID, const std::vector<std::string> &startMsgs, const std::vector<std::string> &endMsgs);

    /*!
     * \brief Updates a set of boxes from Engines coordinates.
     *
     * \param movedBoxes : boxes to be updated
     */
    void updateBoxesFromEngines(const std::vector<unsigned int> &movedBoxes);

    //! The MaquetteScene managing display and interaction.
    MaquetteScene *_scene;

    //! The Engine object managing temporal constraints.
    Engine *_engines;

    //! The map of boxes (identified by IDs) managed by the maquette.
    std::map<unsigned int, BasicBox*> _boxes;

    //! The map of parent boxes (identified by IDs) managed by the maquette.
    std::map<unsigned int, ParentBox*> _parentBoxes;

    //! The map handling a set of control points for each box managed by the maquette.
    std::map<unsigned int, std::pair<unsigned int, unsigned int> > _controlPoints;

    //! The set of relations managed by the maquette.
    std::map<unsigned int, Relation*> _relations;

    //! The set of triggers points managed by the maquette.
    std::map<unsigned int, TriggerPoint*> _triggerPoints;

    //! The next ID to be used for sequential name purpose
//  unsigned int _currentID;
    std::string _currentDevice;

    //! The set of handled devices.
    std::map<std::string, MyDevice> _devices;

    //! The set of available Engines listening ports.
    std::vector<unsigned int> _listeningPorts;
    std::vector<std::string> _plugins;

    QList<BasicBox *> _recordingBoxes;

    //Device _defaultDevice; //!< The default network device used.

    bool _recording;    //!< Handling recording state.
    bool _paused;       //!< Handling paused state.

    QDomDocument *_doc; //!< Handling document used for saving/loading.
};

/*!
 * \brief Callback called when a Trigger Point active state change
 *
 * \param trgID : index of the box's Trigger Point
 * \param waiting : the active state of the Trigger Point
 */
void triggerPointIsActiveCallback(unsigned int trgID, bool active);

/*!
 * \brief Callback called when the running state of a box change.
 *
 * \param boxID : the box whose changing his running status
 * \param running : the new running state of the box
 */
void boxIsRunningCallback(unsigned int boxID, bool running);

/*!
 * \brief Callback called when a transport feature is used
 *
 * \param transpport : a transport feature being used
 * \param value : a value related to the transport
 */
void transportCallback(TTSymbol& transport, const TTValue& value);

/*!
 * \brief Callback called when the execution is finished.
 */
void executionFinishedCallback();
#endif
