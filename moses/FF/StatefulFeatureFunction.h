#pragma once

#include "FeatureFunction.h"

#include "moses/Syntax/SHyperedge.h"

namespace Moses
{
class FFState;

/** base class for all stateful feature functions.
 * eg. LM, distortion penalty
 */
class StatefulFeatureFunction: public FeatureFunction
{
  //All statefull FFs
  static std::vector<const StatefulFeatureFunction*> m_statefulFFs;
  size_t m_statefulId;

public:
  static const std::vector<const StatefulFeatureFunction*>&
  GetStatefulFeatureFunctions() {
    return m_statefulFFs;
  }

  StatefulFeatureFunction(const std::string &line, bool registerNow);
  StatefulFeatureFunction(size_t numScoreComponents, const std::string &line);

  // use this to refer to the state in the hypo vector.
  // for the 1st pass, this is the same as the vector index.
  // but for subsequent passes, you'll need to use this.
  size_t GetStatefulId() const {
    return m_statefulId;
  }

  /**
   * \brief This interface should be implemented.
   * Notes: When evaluating the value of this feature function, you should avoid
   * calling hypo.GetPrevHypo().  If you need something from the "previous"
   * hypothesis, you should store it in an FFState object which will be passed
   * in as prev_state.  If you don't do this, you will get in trouble.
   */
  virtual FFState* EvaluateWhenApplied(
    const Hypothesis& cur_hypo,
    const FFState* prev_state,
    ScoreComponentCollection* accumulator) const = 0;

  virtual FFState* EvaluateWhenAppliedWithContext(
    ttasksptr const& ttasks,
    const Hypothesis& cur_hypo,
    const FFState* prev_state,
    ScoreComponentCollection* accumulator) const {
    return EvaluateWhenApplied(cur_hypo, prev_state, accumulator);
  }

  virtual FFState* EvaluateWhenApplied(
    const ChartHypothesis& /* cur_hypo */,
    int /* featureID - used to index the state in the previous hypotheses */,
    ScoreComponentCollection* accumulator) const = 0;

  virtual FFState* EvaluateWhenApplied(
    const Syntax::SHyperedge& /* cur_hypo */,
    int /* featureID - used to index the state in the previous hypotheses */,
    ScoreComponentCollection* accumulator) const {
    assert(false);
    return 0; /* FIXME */
  }

  //! return the state associated with the empty hypothesis for a given sentence
  virtual const FFState* EmptyHypothesisState(const InputType &input) const = 0;

  bool IsStateless() const {
    return false;
  }

};


}



