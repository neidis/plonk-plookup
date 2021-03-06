#include "note.hpp"

namespace rollup {
namespace prover {

using namespace barretenberg;
using namespace rollup::tx;
using namespace rollup::pedersen_note;

typedef std::pair<private_note, public_note> note_pair;

note_pair create_note_pair(Composer& composer, tx_note const& note)
{
    note_pair result;

    field_ct view_key = witness_ct(&composer, note.secret);
    field_ct note_owner_x = witness_ct(&composer, note.owner.x);
    field_ct note_owner_y = witness_ct(&composer, note.owner.y);
    uint32_ct witness_value = witness_ct(&composer, note.value);
    result.first = { { note_owner_x, note_owner_y }, witness_value, view_key };
    result.second = encrypt_note(result.first);
    return result;
}

void set_note_public(Composer& composer, public_note const& note)
{
    composer.set_public_input(note.ciphertext.x.witness_index);
    composer.set_public_input(note.ciphertext.y.witness_index);
}

byte_array_ct create_note_leaf(Composer& composer, public_note const& note)
{
    byte_array_ct value_byte_array(&composer);
    value_byte_array.write(note.ciphertext.x).write(note.ciphertext.y);
    return value_byte_array;
}

std::string create_note_db_element(public_note const& note)
{
    // TODO: Compress point.
    std::string new_element = std::string(64, 0);
    fr::serialize_to_buffer(note.ciphertext.x.get_value(), (uint8_t*)(&new_element[0]));
    fr::serialize_to_buffer(note.ciphertext.y.get_value(), (uint8_t*)(&new_element[32]));
    return new_element;
}

} // namespace prover
} // namespace rollup