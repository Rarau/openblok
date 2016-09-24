#include "UnitTest++/UnitTest++.h"

#include "game/components/PieceFactory.h"
#include "game/components/Well.h"


struct AppContext {};

SUITE(Well) {

// TODO: get these values from config
constexpr unsigned gravity_delay_frames = 64;
constexpr unsigned lock_delay_frames = 30;

struct WellFixture {
    AppContext app;
    Well well;
    std::string emptyline_ascii;

    WellFixture() {
        for (unsigned i = 0; i < 10; i++)
            emptyline_ascii += '.';
        emptyline_ascii += '\n';
    }
};

TEST_FIXTURE(WellFixture, EmptyOnCreate) {
    std::string expected_ascii;
    for (unsigned i = 0; i < 22; i++)
        expected_ascii += emptyline_ascii;

    CHECK_EQUAL(expected_ascii, well.asAscii());
}

TEST_FIXTURE(WellFixture, FromAscii) {
    std::string expected_ascii;
    for (unsigned i = 0; i < 22; i++) {
        if (i % 3 == 0)
            expected_ascii += emptyline_ascii;
        else
            expected_ascii += "SSSSZZZZTT\n";
    }

    well.fromAscii(expected_ascii);
    CHECK_EQUAL(expected_ascii, well.asAscii());
}

TEST_FIXTURE(WellFixture, AddPiece) {
    CHECK(well.activePiece() == nullptr);
    well.addPiece(Piece::Type::S);
    CHECK(well.activePiece() != nullptr);

    std::string expected_ascii;
    expected_ascii += "....ss....\n";
    expected_ascii += "...ss.....\n";
    for (unsigned i = 0; i < 18; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "....gg....\n";
    expected_ascii += "...gg.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
}

TEST_FIXTURE(WellFixture, Gravity) {
    CHECK(well.activePiece() == nullptr);
    well.addPiece(Piece::Type::S);
    CHECK(well.activePiece() != nullptr);

    // the piece will reach the bottom
    for (unsigned i = 0; i < 20 * gravity_delay_frames; i++)
        well.update({}, app);

    std::string expected_ascii;
    for (unsigned i = 0; i < 20; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "....ss....\n";
    expected_ascii += "...ss.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());

    // it will lock there
    for (unsigned i = 0; i < lock_delay_frames; i++)
        well.update({}, app);

    expected_ascii = "";
    for (unsigned i = 0; i < 20; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "....SS....\n";
    expected_ascii += "...SS.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
    CHECK(well.activePiece() == nullptr);

    // a new piece will appear at the top
    well.addPiece(Piece::Type::Z);
    well.update({}, app);
    CHECK(well.activePiece() != nullptr);

    expected_ascii =  "...zz.....\n";
    expected_ascii += "....zz....\n";
    for (unsigned i = 0; i < 16; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "...gg.....\n";
    expected_ascii += "....gg....\n";
    expected_ascii += "....SS....\n";
    expected_ascii += "...SS.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());

    // and land on top of the previous
    for (unsigned i = 0; i < 19 * gravity_delay_frames; i++)
        well.update({}, app);

    expected_ascii = "";
    for (unsigned i = 0; i < 18; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "...ZZ.....\n";
    expected_ascii += "....ZZ....\n";
    expected_ascii += "....SS....\n";
    expected_ascii += "...SS.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
    CHECK(well.activePiece() == nullptr);
}


TEST_FIXTURE(WellFixture, MoveLeft) {
    well.addPiece(Piece::Type::I);
    well.update({InputEvent(InputType::LEFT, true)}, app);

    std::string expected_ascii = emptyline_ascii;
    expected_ascii += "..iiii....\n";
    for (unsigned i = 0; i < 19; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "..gggg....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
}
TEST_FIXTURE(WellFixture, MoveRight) {
    well.addPiece(Piece::Type::I);
    well.update({InputEvent(InputType::RIGHT, true)}, app);

    std::string expected_ascii = emptyline_ascii;
    expected_ascii += "....iiii..\n";
    for (unsigned i = 0; i < 19; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "....gggg..\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
}
TEST_FIXTURE(WellFixture, MoveDown) {
    well.addPiece(Piece::Type::I);
    well.update({InputEvent(InputType::DOWN, true)}, app);

    std::string expected_ascii = emptyline_ascii;
    expected_ascii += emptyline_ascii;
    expected_ascii += "...iiii...\n";
    for (unsigned i = 0; i < 18; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "...gggg...\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
}

TEST_FIXTURE(WellFixture, Rotate) {
    CHECK(well.activePiece() == nullptr);
    well.addPiece(Piece::Type::S);
    CHECK(well.activePiece() != nullptr);

    well.update({InputEvent(InputType::A, true)}, app);

    std::string expected_ascii = "...s......\n";
    expected_ascii += "...ss.....\n";
    expected_ascii += "....s.....\n";
    for (unsigned i = 0; i < 16; i++)
        expected_ascii += emptyline_ascii;
    expected_ascii += "...g......\n";
    expected_ascii += "...gg.....\n";
    expected_ascii += "....g.....\n";

    CHECK_EQUAL(expected_ascii, well.asAscii());
}

} // Suite
