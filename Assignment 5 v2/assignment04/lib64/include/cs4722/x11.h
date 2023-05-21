#pragma once
#include <GLFW/glfw3.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <GLM/vec4.hpp>


namespace cs4722 {
	class color {
	public:
		GLubyte  r, g, b, a;

		~color() = default;
		
		color(const GLubyte r, const GLubyte g, const GLubyte b)
			: color(r, g, b, 255)
		{
		}


		color(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte a)
			: r(r), g(g), b(b), a(a)
		{
		}

		color() : color(0, 0, 0, 0) {}

//		color(const color& c) = default;

		
		
		color& operator=(const color& x) = default;


		glm::vec4* as_vec4() const;

        /**
         * \brief Return a four tuple of floats that represents this color.
         *
         * Scale the byte values used to represent the components of a color to the range from 0.0 to 1.0.
         * Create an array of these values and return it.
         *
         * \warning
         * This function allocates an array.
         * This can lead to memory leaks unless the array is deleted when not needed any more.
         *
         */
		GLfloat* as_float_array() const;

        /**
         * \brief Return a four tuple of floats that represents this color.
         *      The float is packaged in a <code>unique_ptr</code> which will take
         *      care of deleting the allocated storage when no longer needed.
         *
         *  This function calls as_float_array(), wraps the array, and returns the unique_ptr.
         *
         */
        std::unique_ptr<GLfloat[]> as_float() const;

        /**
         * \brief This function converts a color from internal format to float in the same manner as
         *      <code>as_float</code>.
         *      The resulting values are put in the argument.
         *
         *  \warning  This function is <strong>dangerous</strong>.
         *      If an array with fewer than four elements is passed as the parameter
         *      <code>color</code> then this function will try to store values
         *      in the memory following the array.
         *      This leads to undefined behavior.
         *      If you are lucky, the program will crash during testing.
         *      If you are not so lucky, you will be reading in the news about how your program was hacked to
         *      allow access to the data for thousands of users.
         *
         *
         *   Why would we provide such a dangerous function?
         *   Because it supports higher performance.
         *   The other 'as_float' functions allocate arrays each time they are executed.
         *   Those arrays must be deleted.
         *   For material colors this will happen over and over as the scene is displayed.
         *   It is quicker to allocate the storage once, before the program starts running, and
         *   then just copy the appropriate values into place.
         *
         * @param color  An array of at least 4 elements.
         */
        void as_float(GLfloat* color) const;


//		explicit operator std::string() const;

//		friend std::ostream& operator<<(std::ostream& output, const color& c);

//        color operator*(const color& y);


	};


	class x11 {
	public:
		static  color alice_blue;
		static  color antique_white;
		static  color antique_white1;
		static  color antique_white2;
		static  color antique_white3;
		static  color antique_white4;
		static  color aquamarine;
		static  color aquamarine1;
		static  color aquamarine2;
		static  color aquamarine3;
		static  color aquamarine4;
		static  color azure;
		static  color azure1;
		static  color azure2;
		static  color azure3;
		static  color azure4;
		static  color beige;
		static  color bisque;
		static  color bisque1;
		static  color bisque2;
		static  color bisque3;
		static  color bisque4;
		static  color black;
		static  color blanched_almond;
		static  color blue;
		static  color blue1;
		static  color blue2;
		static  color blue3;
		static  color blue4;
		static  color blue_violet;
		static  color brown;
		static  color brown1;
		static  color brown2;
		static  color brown3;
		static  color brown4;
		static  color burlywood;
		static  color burlywood1;
		static  color burlywood2;
		static  color burlywood3;
		static  color burlywood4;
		static  color cadet_blue;
		static  color cadet_blue1;
		static  color cadet_blue2;
		static  color cadet_blue3;
		static  color cadet_blue4;
		static  color chartreuse;
		static  color chartreuse1;
		static  color chartreuse2;
		static  color chartreuse3;
		static  color chartreuse4;
		static  color chocolate;
		static  color chocolate1;
		static  color chocolate2;
		static  color chocolate3;
		static  color chocolate4;
		static  color coral;
		static  color coral1;
		static  color coral2;
		static  color coral3;
		static  color coral4;
		static  color cornflower_blue;
		static  color cornsilk;
		static  color cornsilk1;
		static  color cornsilk2;
		static  color cornsilk3;
		static  color cornsilk4;
		static  color cyan;
		static  color cyan1;
		static  color cyan2;
		static  color cyan3;
		static  color cyan4;
		static  color dark_blue;
		static  color dark_cyan;
		static  color dark_goldenrod;
		static  color dark_goldenrod1;
		static  color dark_goldenrod2;
		static  color dark_goldenrod3;
		static  color dark_goldenrod4;
		static  color dark_gray;
		static  color dark_green;
		static  color dark_grey;
		static  color dark_khaki;
		static  color dark_magenta;
		static  color dark_olive_green;
		static  color dark_olive_green1;
		static  color dark_olive_green2;
		static  color dark_olive_green3;
		static  color dark_olive_green4;
		static  color dark_orange;
		static  color dark_orange1;
		static  color dark_orange2;
		static  color dark_orange3;
		static  color dark_orange4;
		static  color dark_orchid;
		static  color dark_orchid1;
		static  color dark_orchid2;
		static  color dark_orchid3;
		static  color dark_orchid4;
		static  color dark_red;
		static  color dark_salmon;
		static  color dark_sea_green;
		static  color dark_sea_green1;
		static  color dark_sea_green2;
		static  color dark_sea_green3;
		static  color dark_sea_green4;
		static  color dark_slate_blue;
		static  color dark_slate_gray;
		static  color dark_slate_gray1;
		static  color dark_slate_gray2;
		static  color dark_slate_gray3;
		static  color dark_slate_gray4;
		static  color dark_slate_grey;
		static  color dark_turquoise;
		static  color dark_violet;
		static  color deep_pink;
		static  color deep_pink1;
		static  color deep_pink2;
		static  color deep_pink3;
		static  color deep_pink4;
		static  color deep_sky_blue;
		static  color deep_sky_blue1;
		static  color deep_sky_blue2;
		static  color deep_sky_blue3;
		static  color deep_sky_blue4;
		static  color dim_gray;
		static  color dim_grey;
		static  color dodger_blue;
		static  color dodger_blue1;
		static  color dodger_blue2;
		static  color dodger_blue3;
		static  color dodger_blue4;
		static  color firebrick;
		static  color firebrick1;
		static  color firebrick2;
		static  color firebrick3;
		static  color firebrick4;
		static  color floral_white;
		static  color forest_green;
		static  color gainsboro;
		static  color ghost_white;
		static  color gold;
		static  color gold1;
		static  color gold2;
		static  color gold3;
		static  color gold4;
		static  color goldenrod;
		static  color goldenrod1;
		static  color goldenrod2;
		static  color goldenrod3;
		static  color goldenrod4;
		static  color gray;
		static  color gray0;
		static  color gray1;
		static  color gray10;
		static  color gray100;
		static  color gray11;
		static  color gray12;
		static  color gray13;
		static  color gray14;
		static  color gray15;
		static  color gray16;
		static  color gray17;
		static  color gray18;
		static  color gray19;
		static  color gray2;
		static  color gray20;
		static  color gray21;
		static  color gray22;
		static  color gray23;
		static  color gray24;
		static  color gray25;
		static  color gray26;
		static  color gray27;
		static  color gray28;
		static  color gray29;
		static  color gray3;
		static  color gray30;
		static  color gray31;
		static  color gray32;
		static  color gray33;
		static  color gray34;
		static  color gray35;
		static  color gray36;
		static  color gray37;
		static  color gray38;
		static  color gray39;
		static  color gray4;
		static  color gray40;
		static  color gray41;
		static  color gray42;
		static  color gray43;
		static  color gray44;
		static  color gray45;
		static  color gray46;
		static  color gray47;
		static  color gray48;
		static  color gray49;
		static  color gray5;
		static  color gray50;
		static  color gray51;
		static  color gray52;
		static  color gray53;
		static  color gray54;
		static  color gray55;
		static  color gray56;
		static  color gray57;
		static  color gray58;
		static  color gray59;
		static  color gray6;
		static  color gray60;
		static  color gray61;
		static  color gray62;
		static  color gray63;
		static  color gray64;
		static  color gray65;
		static  color gray66;
		static  color gray67;
		static  color gray68;
		static  color gray69;
		static  color gray7;
		static  color gray70;
		static  color gray71;
		static  color gray72;
		static  color gray73;
		static  color gray74;
		static  color gray75;
		static  color gray76;
		static  color gray77;
		static  color gray78;
		static  color gray79;
		static  color gray8;
		static  color gray80;
		static  color gray81;
		static  color gray82;
		static  color gray83;
		static  color gray84;
		static  color gray85;
		static  color gray86;
		static  color gray87;
		static  color gray88;
		static  color gray89;
		static  color gray9;
		static  color gray90;
		static  color gray91;
		static  color gray92;
		static  color gray93;
		static  color gray94;
		static  color gray95;
		static  color gray96;
		static  color gray97;
		static  color gray98;
		static  color gray99;
		static  color green;
		static  color green1;
		static  color green2;
		static  color green3;
		static  color green4;
		static  color green_yellow;
		static  color grey;
		static  color grey0;
		static  color grey1;
		static  color grey10;
		static  color grey100;
		static  color grey11;
		static  color grey12;
		static  color grey13;
		static  color grey14;
		static  color grey15;
		static  color grey16;
		static  color grey17;
		static  color grey18;
		static  color grey19;
		static  color grey2;
		static  color grey20;
		static  color grey21;
		static  color grey22;
		static  color grey23;
		static  color grey24;
		static  color grey25;
		static  color grey26;
		static  color grey27;
		static  color grey28;
		static  color grey29;
		static  color grey3;
		static  color grey30;
		static  color grey31;
		static  color grey32;
		static  color grey33;
		static  color grey34;
		static  color grey35;
		static  color grey36;
		static  color grey37;
		static  color grey38;
		static  color grey39;
		static  color grey4;
		static  color grey40;
		static  color grey41;
		static  color grey42;
		static  color grey43;
		static  color grey44;
		static  color grey45;
		static  color grey46;
		static  color grey47;
		static  color grey48;
		static  color grey49;
		static  color grey5;
		static  color grey50;
		static  color grey51;
		static  color grey52;
		static  color grey53;
		static  color grey54;
		static  color grey55;
		static  color grey56;
		static  color grey57;
		static  color grey58;
		static  color grey59;
		static  color grey6;
		static  color grey60;
		static  color grey61;
		static  color grey62;
		static  color grey63;
		static  color grey64;
		static  color grey65;
		static  color grey66;
		static  color grey67;
		static  color grey68;
		static  color grey69;
		static  color grey7;
		static  color grey70;
		static  color grey71;
		static  color grey72;
		static  color grey73;
		static  color grey74;
		static  color grey75;
		static  color grey76;
		static  color grey77;
		static  color grey78;
		static  color grey79;
		static  color grey8;
		static  color grey80;
		static  color grey81;
		static  color grey82;
		static  color grey83;
		static  color grey84;
		static  color grey85;
		static  color grey86;
		static  color grey87;
		static  color grey88;
		static  color grey89;
		static  color grey9;
		static  color grey90;
		static  color grey91;
		static  color grey92;
		static  color grey93;
		static  color grey94;
		static  color grey95;
		static  color grey96;
		static  color grey97;
		static  color grey98;
		static  color grey99;
		static  color honeydew;
		static  color honeydew1;
		static  color honeydew2;
		static  color honeydew3;
		static  color honeydew4;
		static  color hot_pink;
		static  color hot_pink1;
		static  color hot_pink2;
		static  color hot_pink3;
		static  color hot_pink4;
		static  color indian_red;
		static  color indian_red1;
		static  color indian_red2;
		static  color indian_red3;
		static  color indian_red4;
		static  color ivory;
		static  color ivory1;
		static  color ivory2;
		static  color ivory3;
		static  color ivory4;
		static  color khaki;
		static  color khaki1;
		static  color khaki2;
		static  color khaki3;
		static  color khaki4;
		static  color lavender;
		static  color lavender_blush;
		static  color lavender_blush1;
		static  color lavender_blush2;
		static  color lavender_blush3;
		static  color lavender_blush4;
		static  color lawn_green;
		static  color lemon_chiffon;
		static  color lemon_chiffon1;
		static  color lemon_chiffon2;
		static  color lemon_chiffon3;
		static  color lemon_chiffon4;
		static  color light_blue;
		static  color light_blue1;
		static  color light_blue2;
		static  color light_blue3;
		static  color light_blue4;
		static  color light_coral;
		static  color light_cyan;
		static  color light_cyan1;
		static  color light_cyan2;
		static  color light_cyan3;
		static  color light_cyan4;
		static  color light_goldenrod;
		static  color light_goldenrod1;
		static  color light_goldenrod2;
		static  color light_goldenrod3;
		static  color light_goldenrod4;
		static  color light_goldenrod_yellow;
		static  color light_gray;
		static  color light_green;
		static  color light_grey;
		static  color light_pink;
		static  color light_pink1;
		static  color light_pink2;
		static  color light_pink3;
		static  color light_pink4;
		static  color light_salmon;
		static  color light_salmon1;
		static  color light_salmon2;
		static  color light_salmon3;
		static  color light_salmon4;
		static  color light_sea_green;
		static  color light_sky_blue;
		static  color light_sky_blue1;
		static  color light_sky_blue2;
		static  color light_sky_blue3;
		static  color light_sky_blue4;
		static  color light_slate_blue;
		static  color light_slate_gray;
		static  color light_slate_grey;
		static  color light_steel_blue;
		static  color light_steel_blue1;
		static  color light_steel_blue2;
		static  color light_steel_blue3;
		static  color light_steel_blue4;
		static  color light_yellow;
		static  color light_yellow1;
		static  color light_yellow2;
		static  color light_yellow3;
		static  color light_yellow4;
		static  color lime_green;
		static  color linen;
		static  color magenta;
		static  color magenta1;
		static  color magenta2;
		static  color magenta3;
		static  color magenta4;
		static  color maroon;
		static  color maroon1;
		static  color maroon2;
		static  color maroon3;
		static  color maroon4;
		static  color medium_aquamarine;
		static  color medium_blue;
		static  color medium_orchid;
		static  color medium_orchid1;
		static  color medium_orchid2;
		static  color medium_orchid3;
		static  color medium_orchid4;
		static  color medium_purple;
		static  color medium_purple1;
		static  color medium_purple2;
		static  color medium_purple3;
		static  color medium_purple4;
		static  color medium_sea_green;
		static  color medium_slate_blue;
		static  color medium_spring_green;
		static  color medium_turquoise;
		static  color medium_violet_red;
		static  color midnight_blue;
		static  color mint_cream;
		static  color misty_rose;
		static  color misty_rose1;
		static  color misty_rose2;
		static  color misty_rose3;
		static  color misty_rose4;
		static  color moccasin;
		static  color navajo_white;
		static  color navajo_white1;
		static  color navajo_white2;
		static  color navajo_white3;
		static  color navajo_white4;
		static  color navy;
		static  color navy_blue;
		static  color old_lace;
		static  color olive_drab;
		static  color olive_drab1;
		static  color olive_drab2;
		static  color olive_drab3;
		static  color olive_drab4;
		static  color orange;
		static  color orange1;
		static  color orange2;
		static  color orange3;
		static  color orange4;
		static  color orange_red;
		static  color orange_red1;
		static  color orange_red2;
		static  color orange_red3;
		static  color orange_red4;
		static  color orchid;
		static  color orchid1;
		static  color orchid2;
		static  color orchid3;
		static  color orchid4;
		static  color pale_goldenrod;
		static  color pale_green;
		static  color pale_green1;
		static  color pale_green2;
		static  color pale_green3;
		static  color pale_green4;
		static  color pale_turquoise;
		static  color pale_turquoise1;
		static  color pale_turquoise2;
		static  color pale_turquoise3;
		static  color pale_turquoise4;
		static  color pale_violet_red;
		static  color pale_violet_red1;
		static  color pale_violet_red2;
		static  color pale_violet_red3;
		static  color pale_violet_red4;
		static  color papaya_whip;
		static  color peach_puff;
		static  color peach_puff1;
		static  color peach_puff2;
		static  color peach_puff3;
		static  color peach_puff4;
		static  color peru;
		static  color pink;
		static  color pink1;
		static  color pink2;
		static  color pink3;
		static  color pink4;
		static  color plum;
		static  color plum1;
		static  color plum2;
		static  color plum3;
		static  color plum4;
		static  color powder_blue;
		static  color purple;
		static  color purple1;
		static  color purple2;
		static  color purple3;
		static  color purple4;
		static  color red;
		static  color red1;
		static  color red2;
		static  color red3;
		static  color red4;
		static  color rosy_brown;
		static  color rosy_brown1;
		static  color rosy_brown2;
		static  color rosy_brown3;
		static  color rosy_brown4;
		static  color royal_blue;
		static  color royal_blue1;
		static  color royal_blue2;
		static  color royal_blue3;
		static  color royal_blue4;
		static  color saddle_brown;
		static  color salmon;
		static  color salmon1;
		static  color salmon2;
		static  color salmon3;
		static  color salmon4;
		static  color sandy_brown;
		static  color sea_green;
		static  color sea_green1;
		static  color sea_green2;
		static  color sea_green3;
		static  color sea_green4;
		static  color seashell;
		static  color seashell1;
		static  color seashell2;
		static  color seashell3;
		static  color seashell4;
		static  color sienna;
		static  color sienna1;
		static  color sienna2;
		static  color sienna3;
		static  color sienna4;
		static  color sky_blue;
		static  color sky_blue1;
		static  color sky_blue2;
		static  color sky_blue3;
		static  color sky_blue4;
		static  color slate_blue;
		static  color slate_blue1;
		static  color slate_blue2;
		static  color slate_blue3;
		static  color slate_blue4;
		static  color slate_gray;
		static  color slate_gray1;
		static  color slate_gray2;
		static  color slate_gray3;
		static  color slate_gray4;
		static  color slate_grey;
		static  color snow;
		static  color snow1;
		static  color snow2;
		static  color snow3;
		static  color snow4;
		static  color spring_green;
		static  color spring_green1;
		static  color spring_green2;
		static  color spring_green3;
		static  color spring_green4;
		static  color steel_blue;
		static  color steel_blue1;
		static  color steel_blue2;
		static  color steel_blue3;
		static  color steel_blue4;
		static  color tan;
		static  color tan1;
		static  color tan2;
		static  color tan3;
		static  color tan4;
		static  color thistle;
		static  color thistle1;
		static  color thistle2;
		static  color thistle3;
		static  color thistle4;
		static  color tomato;
		static  color tomato1;
		static  color tomato2;
		static  color tomato3;
		static  color tomato4;
		static  color turquoise;
		static  color turquoise1;
		static  color turquoise2;
		static  color turquoise3;
		static  color turquoise4;
		static  color violet;
		static  color violet_red;
		static  color violet_red1;
		static  color violet_red2;
		static  color violet_red3;
		static  color violet_red4;
		static  color wheat;
		static  color wheat1;
		static  color wheat2;
		static  color wheat3;
		static  color wheat4;
		static  color white;
		static  color white_smoke;
		static  color yellow;
		static  color yellow1;
		static  color yellow2;
		static  color yellow3;
		static  color yellow4;
		static  color yellow_green;


		



	};

}
