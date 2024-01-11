// Copyright 2019-2023 Cambridge Quantum Computing
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Transform.hpp"

namespace tket {

namespace Transforms {

//////////////////////////
// Full Optimisation Pass//
//////////////////////////

/*these Transform passes do not preserve connectivity*/

/**
 * only peephole optimisation, so no higher structure abstraction.
 * Two qubit Cartan, Clifford, synthesis
 * @param allow_swaps whether to allow introduction of implicit wire swaps
 * Expects: Any gates
 * Produces: CX, TK1
 *
 */
Transform peephole_optimise_2q(bool allow_swaps = true);

/**
 * Peephole optimisation including resynthesis of three-qubit gate sequences.
 *
 * The allow_swaps parameter has no effect when the target gate is TK2.
 *
 * @param allow_swaps whether to allow introduction of implicit wire swaps
 * @param target_2qb_gate target 2-qubut gate (CX or TK2)
 *
 * Produces: (CX or TK2) and TK1.
 */
Transform full_peephole_optimise(
    bool allow_swaps = true, OpType target_2qb_gate = OpType::CX);

/**
 * Simplify using ZX calculus and extract a circuit back out.
 *
 * Will not work if the circuit contains created or discarded qubits.
 *
 * This may increase the cost of the circuit.
 */
Transform zx_graphlike_optimisation();

typedef std::function<bool(const Circuit&, const Circuit&)> AcceptanceCriterion;

/**
 * Apply a rebase to {Rx, Rz, X, Z, H, CZ, CX} followed by
 * \ref zx_graphlike_optimisation; discard the result if the given criterion is
 * not satisfied.
 *
 * \param criterion Function taking the old circuit and the new circuit (in that
 * order) which returns true if we want to accept the new circuit.
 */
Transform try_zx_graphlike_optimisation(const AcceptanceCriterion& criterion);

// kitchen sink optimisation - phase gadget resynthesis, two-qubit Cartan
// forms, Clifford Expects: Any gates Produces: CX, TK1
Transform canonical_hyper_clifford_squash();

/**
 * runs clifford_simp
 * @param allow_swaps whether to allow introduction of implicit wire swaps
 * Expects: Any gates
 * Produces: CX, TK1
 */
Transform hyper_clifford_squash(bool allow_swaps = true);

// simplifies a circuit using Clifford rules
// Expects: CX and any single-qubit gates
// Produces: CX, TK1
Transform clifford_simp(bool allow_swaps = true);

//////////////////
// Synthesis Pass//
//////////////////

/*Synthesis passes preserve connectivity */

/**
 * Synthesise a circuit consisting of TK2 and TK1 gates only.
 */
Transform synthesise_tk();

/**
 * Synthesise a circuit consisting of CX and TK1 gates only.
 */
Transform synthesise_tket();

// converts a circuit into the OQC primitives (Rz, SX, ECR gates)
// Expects: any gates
// Produces: Rz, SX, ECR
Transform synthesise_OQC();

// converts a circuit into the HQS primitives (Rz, PhasedX, ZZMax) whilst
// optimising Expects: CX and any single-qubit gates Produces: ZZMax, PhasedX,
// Rz
Transform synthesise_HQS();

// converts a circuit into the UMD primitives (Rz, PhasedX, XXPhase) whilst
// optimising Expects: Any gate set Produces: XXPhase, PhasedX, Rz
Transform synthesise_UMD();

/////////////////////////////
// Pauli Gadget Optimisation//
/////////////////////////////

/**
 * Depth-saving resynthesis of phase gadgets with alignment.
 *
 * Produces CX and TK1 gates.
 */
Transform optimise_via_PhaseGadget(
    CXConfigType cx_config = CXConfigType::Snake);

}  // namespace Transforms

}  // namespace tket
