#include <gtest/gtest.h>

#include <unicode/cpaf_u8string_utils.h>
#include <unicode/cpaf_u16string_utils.h>

using namespace testing;

const std::string to_u16_1 = R"(
<H2>Beskrivelse</H2> <H2><STRONG>Booster X5 – 5000 lumen LED cykellygte</STRONG> </H2> <P> Bestseller. Super god Led cykellygte som ikke kan findes billigere eller bedre på markedet.</P> <P> Dette er en lovlig super LED Highpower forlygte, normalt har en forlygte 1 eller 2 lysdioder, men denne har 5 highpower dioder af hver 1000 lumen og giver derfor et langt kraftigere lys i mørket, hvilket selvfølgeligt er ret perfekt.</P> <P> Vores Booster X5 LED cykellygte er perfekt til bl.a. MTB, landevejscykling, orienteringsløb og andre udendørsativiteter.<BR> Lygten har 3 funktioner hhv. super highpower, highpower og en blinkefunktion.</P> <H1>Hvad er Fastmount:</H1> Her får du et batteri med den nemmeste og sikreste montering. Vi har med Fastmount tilstræbt en montering af batteriet, som både er let at have med at gøre, sikker i forhold til de hårde vilkår på MTB sporet og som ikke ridser, eller sætter slidmærker på cyklen. Fastmount er et IP65 godkendt støv- og vandtæt batteri, der er indkapslet i en lukket boks med kraftige gummiremme og nylonstraps. Til monteringen medfølger et stykke kraftigt folie/film til at sætte omkring cykelstellet, sådan at dette ikke ridses selvom der kommer vand, grus og mudder imellem batteriet og stellet. Vi har stor tiltro til disse nye batterier, ikke mindst fordi selve battericellerne er den samme som vi altid har benyttet i vores XL pakke. Men nu er de pakket ind i denne fixe kasse, som sikrer bedre montering på cyklen. Som sædvanligt er det markedets største batteripakke, XL med 13200 mAh vi leverer. Større end, hvad alle andre på markedet leverer. Fordi det værste er at gå tør for strøm. <H3><STRONG>Hvad medfølger?</STRONG> </H3> <P> Der medfølger: Forlygte, batteripakke, O-ringe, pandebånd til lygten, oplader.</P> <H3><STRONG>Ekstra information</STRONG> </H3> <TR><TD> Lumen:</TD> <TD> 5000 lumen, lygten har 5 dioder af 1000 lumen hver.Er du I tvivl om lumen? læs her </TD> </TR> <TR><TD> Er lygten lovlig?</TD> <TD> Ja, den nyeste lovgivning kræver batteritid på minimum 5 timer, samt at kraftige lygter skal pege lidt nedad for ikke at blænde modkørende.<BR> Her kan du læse om den nyeste lovgivning. </TD> </TR> <TR><TD> Er der garanti?</TD> <TD> Alle vores lygter er testet igennem flere år og vi yder derfor 1 års garanti.</TD> </TR> <TR><TD> Nem montering?</TD> <TD> Monteringen er super nem, det foregår vha. en elastik rundt om f.eks. styret eller pandebåndet.</TD> </TR> <TR><TD> Batteritid:</TD> <TD> Ved aktuel test har lygten lyst mere end 9 timer og 15 minutter ved laveste konstante lysstyrke (Lovkrav er minimum 5 timer). Ved fuld lysstyrke har den lyst mere end 3 timer og 30 minutter. Flash funktionen lyser 7 timer og 30 minutter. Batteriet er specificeret til at kunne klare 500 op- og afladninger, men grundet den store batteristørrelse stresses batteriet langt mindre end normalt med mindre batterier.<P> Der medfølger vores vandtætte Fastmount XL Batteripakke (8,4V/13200 mAh)</TD> </TR> <TR><TD> Vægt:</TD> <TD> 138g (lygtehoved).</TD> </TR> <TR><TD> Hvad er lygten ideel til?</TD> <TD> Denne lygte er vores bestseller til blandt andet MTB ryttere. Den leverer rigtigt meget lys. Godt i den mærke skov 😉 Men kan også benyttes af orienteringsløberen, samt af landevejscyklisten.</TD> </TR> <TR><TD> Vedligeholdelse</TD> <TD> Her får du er vandtæt lygtesæt som ikke behøver vedligeholdelse. Dog bør du tørre lygter og batteripakke af med en tør klud efter regnvejr.</TD> </TR> <H1></H1>
)";

class UnicodeUnitTest : public testing::Test
{
public:
    UnicodeUnitTest()  = default;

    ~UnicodeUnitTest() override = default;

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(UnicodeUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
