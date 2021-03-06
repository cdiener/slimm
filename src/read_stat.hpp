// ==========================================================================
//    SLIMM - Species Level Identification of Microbes from Metagenomes.
// ==========================================================================
// Copyright (c) 2014-2017, Temesgen H. Dadi, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Temesgen H. Dadi or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL TEMESGEN H. DADI OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Temesgen H. Dadi <temesgen.dadi@fu-berlin.de>
// ==========================================================================

#ifndef READ_STAT_H
#define READ_STAT_H

using namespace seqan;

// ==========================================================================
// Classes
// ==========================================================================
// ----------------------------------------------------------------------------
// Class target_reference
// ----------------------------------------------------------------------------
class target_reference
{
public:
    uint32_t                   reference_id;
    std::vector<uint32_t>      positions;

    //constructer takes a ref id and a position for the first time
    target_reference(uint32_t ref, uint32_t pos)
    {
        reference_id = ref;
        positions.reserve(10);
        positions.push_back(pos);
    }
};


// ----------------------------------------------------------------------------
// Class read_stat
// ----------------------------------------------------------------------------
class read_stat
{
public:
    std::vector<target_reference>   targets;
    uint32_t                        refs_length_sum = 0;

    //checks if all the match points are in the same sequence
    bool is_uniq()
    {
        return (targets.size() == 1);
    }

    // checks if all the match points are in the same sequence
    // ignoring sequences that are not in valid_ref_ids
    bool is_uniq(std::set<uint32_t> const & valid_ref_ids)
    {
        if (targets.size() == 1)
            return true;
        else
        {
            uint32_t ref_count = 0;
            for (auto tr : targets)
            {
                if (valid_ref_ids.find(tr.reference_id) != valid_ref_ids.end()) // if valid ref_id
                    ++ref_count;
                if (ref_count > 1)
                    return false;
            }
        }
        return true;
    }

    // remove targets of masked_ref_ids
    void update(std::set<uint32_t> const & valid_ref_ids, std::vector<reference_contig> const & references)
    {
        if (targets.size() == 0)
            return;
        else
        {
            std::vector<target_reference> new_targets;
            for (auto tr : targets)
            {
                if (valid_ref_ids.find(tr.reference_id) != valid_ref_ids.end()) // if valid ref_id
                    new_targets.push_back(tr);
                else
                    refs_length_sum -= references[tr.reference_id].length;
            }
            std::swap(targets, new_targets);
        }
    }

    void add_target(int32_t reference_id, uint32_t bin_number)
    {
        if (targets.empty())
        {
            targets.push_back(target_reference(reference_id, bin_number));
            return;
        }
        else
        {
            for (auto tar : targets)
            {
                if(tar.reference_id == reference_id)
                {
                    tar.positions.push_back(bin_number);
                    return;
                }
            }
            targets.push_back(target_reference(reference_id, bin_number));
        }
    }
};

#endif /* READ_STAT_H */
