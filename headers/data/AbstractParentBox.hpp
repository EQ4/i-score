/*
 * Copyright: LaBRI / SCRIME
 *
 * Authors: Luc Vercellin (17/05/2010)
 *
 * luc.vercellin@labri.fr
 *
 * This software is a computer program whose purpose is to provide
 * notation/composition combining synthesized as well as recorded
 * sounds, providing answers to the problem of notation and, drawing,
 * from its very design, on benefits from state of the art research
 * in musicology and sound/music computing.
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
#ifndef ABSTRACT_PARENT_BOX
#define ABSTRACT_PARENT_BOX

/*!
 * \file AbstractParentBox.hpp
 *
 * \author Luc Vercellin
 */

#include "AbstractBox.hpp"
#include <string>
#include "Engine.h"

//! Defines abstract parent box type.
enum { ABSTRACT_PARENT_BOX_TYPE = 7 };

/*!
 * \class AbstractParentBox
 *
 * \brief Class managing main information about a parent box.
 */
class AbstractParentBox : public AbstractBox
{
  public:
    friend class ParentBox; /// \todo vérifier l'implication de friend class. (par jaime Chao)

    AbstractParentBox(const QPointF &newTopLeft = QPointF(0., 0.), const float &newLength = 0., const float &newHeight = 0.,
                      const std::string &newName = "", const QColor &newColor = Qt::black, unsigned int ID = NO_ID, unsigned int motherID = ROOT_BOX_ID);

    AbstractParentBox(const AbstractParentBox &other);

    AbstractParentBox(const AbstractBox &other);

    inline virtual
    ~AbstractParentBox(){}

    /*!
     * \brief Gets the type of the abstract item.
     *
     * \return the type of the abstract item.
     */
    int type() const;

  protected:
};
#endif
