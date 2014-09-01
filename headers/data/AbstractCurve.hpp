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
#ifndef ABSTRACT_CURVE_H
#define ABSTRACT_CURVE_H

/*!
 * \file AbstractCurve.h
 *
 * \author Luc Vercellin
 */

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <map>
#include "Abstract.hpp"
#include "Engine.h"
#include <QPoint>
#include <QColor>
#include <math.h>

class QColor;

//! Defines abstract parent box type.
enum { ABSTRACT_CURVE_TYPE = 8 }; /// \todo Jamais utilisé dans le projet! Et devrait être rangé dans la classe.(par jaime Chao)

/*!
 * \class AbstractCurve
 *
 * \brief Class managing main information about a curve.
 */
class AbstractCurve : public Abstract
{
  public:
    AbstractCurve(unsigned int boxID, const std::string &address, unsigned int argPosition, unsigned int sampleRate,
                  bool redundancy, bool show, bool interpolate, float lastPointCoeff, const std::vector<float> &curve,
                  const std::map<float, std::pair<float, float> > &breakpoints);

    AbstractCurve(const AbstractCurve &other);

    virtual
    ~AbstractCurve(){}

    virtual int type() const; /// \todo les types sont utiles aux GraphicsItem. (par jaime Chao)

  private:
    unsigned int _boxID;      //!< Box ID.
    std::string _address;     //!< Address of the curve.
    unsigned int _argPosition;
    unsigned int _sampleRate; //!< Curve sample rate.
    bool _redundancy;         //!< Handles curve's redundancy
    bool _show;
    bool _interpolate;
    std::vector<float> _curve; //!< List of all curve values.
    //! Map of breakpoints with their values and curving values.
    std::map<float, std::pair<float, float> > _breakpoints;

    friend class CurveWidget; /// \todo vérifier l'implication de friend class. (par jaime Chao)
    friend class CurvesWidget; /// \todo vérifier l'implication de friend class
    friend class BoxWidget; /// \todo vérifier l'implication de friend class
};
#endif
