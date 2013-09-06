/*
Copyright (c) 2012, UT-Battelle, LLC
All rights reserved

[MPS++, Version 0.1]
[by K. Al-Hassanieh, Oak Ridge National Laboratory]
[by J. Rincon, Oak Ridge National Laboratory]
[by G.A., Oak Ridge National Laboratory]

See full open source LICENSE under file LICENSE
in the root directory of this distribution.

*********************************************************
DISCLAIMER

THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*********************************************************

*/
/** \ingroup Dmrg */
/*@{*/

#ifndef MODEL_SELECTOR_H
#define MODEL_SELECTOR_H

#include "String.h"
#include <stdexcept>
#include "../Models/HubbardOneBand/ModelHubbard.h"
#include "../Models/HeisenbergSpinOneHalf/ModelHeisenberg.h"
#include "../Models/ExtendedHubbard1Orb/ExtendedHubbard1Orb.h"
#include "../Models/FeAsModel/ModelFeBasedSc.h"
#include "../Models/FeAsBasedScExtended/FeAsBasedScExtended.h"
#include "../Models/Immm/Immm.h"
#include "../Models/Tj1Orb/Tj1Orb.h"

namespace Dmrg {

template<typename ModelBaseType>
class ModelSelector {

	typedef typename ModelBaseType::ModelHelperType ModelHelperType;
	typedef typename ModelBaseType::SolverParamsType SolverParamsType;
	typedef typename ModelBaseType::GeometryType GeometryType;
	typedef typename ModelBaseType::InputValidatorType InputValidatorType;
	typedef typename ModelHelperType::SparseMatrixType SparseMatrixType;

	// start models here:
	typedef ModelHubbard<ModelBaseType> ModelHubbardType;
	typedef ModelHeisenberg<ModelBaseType> ModelHeisenbergType;
	typedef ExtendedHubbard1Orb<ModelBaseType> ModelHubbardExtType;
	typedef ModelFeBasedSc<ModelBaseType> FeBasedScType;
	typedef FeAsBasedScExtended<ModelBaseType> FeBasedScExtType;
	typedef Immm<ModelBaseType> ImmmType;
	typedef Tj1Orb<ModelBaseType> Tj1OrbType;
	// end models

public:

	ModelSelector(const PsimagLite::String& name)
	: name_(name),model_(0)
	{}

	~ModelSelector()
	{
		if (model_) delete model_;
	}

	const ModelBaseType& operator()(const SolverParamsType& solverParams,
	                                InputValidatorType& io,
	                                const GeometryType& geometry)
	{
		if (model_) return *model_;

		if (name_ == "HubbardOneBand") {
			model_ = new ModelHubbardType(solverParams,io,geometry);
		} else if (name_ == "HeisenbergSpinOneHalf") {
			model_ = new ModelHeisenbergType(solverParams,io,geometry);
		} else if (name_ == "HubbardOneBandExtended") {
			model_ = new ModelHubbardExtType(solverParams,io,geometry);
		} else  if (name_ == "FeAsBasedSc") {
			model_ = new FeBasedScType(solverParams,io,geometry);
		} else if (name_ == "FeAsBasedScExtended") {
			model_ = new FeBasedScExtType(solverParams,io,geometry);
		} else if (name_ == "Immm") {
			model_ = new ImmmType(solverParams,io,geometry);
		} else if (name_ == "Tj1Orb") {
			model_ = new Tj1OrbType(solverParams,io,geometry);
		} else {
			PsimagLite::String s(__FILE__);
			s += " Unknown model " + name_ + "\n";
			throw PsimagLite::RuntimeError(s.c_str());
		}

		ProgramGlobals::init(model_->hilbertSize(0),
		                     model_->geometry().numberOfSites());

		return *model_;
	}

private:

	PsimagLite::String name_;
	ModelBaseType* model_;

}; // ModelSelector

} // namespace Dmrg

/*@}*/
#endif // MODEL_SELECTOR_H
