/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 
 <TODO: Add in GPLv2-notice, need to make sure if we are v2-only, or v2-or-later,
 we are atleast v2>
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FITD_CVARS_H
#define FITD_CVARS_H

namespace Fitd {

enum enumCVars
{
	SAMPLE_PAGE = 0,
	BODY_FLAMME = 1,
	MAX_WEIGHT_LOADABLE = 2,
	TEXTE_CREDITS = 3,
	SAMPLE_TONNERRE  = 4,
	INTRO_DETECTIVE = 5,
	INTRO_HERITIERE = 6,
	WORLD_NUM_PERSO = 7,
	CHOOSE_PERSO = 8,
	SAMPLE_CHOC = 9,
	SAMPLE_PLOUF = 10,
	REVERSE_OBJECT = 11,
	KILLED_SORCERER = 12,
	LIGHT_OBJECT = 13,
	FOG_FLAG = 14,
	DEAD_PERSO = 15,
	JET_SARBACANE,
	TIR_CANON,
	JET_SCALPEL,
	POIVRE,
	DORTOIR,
	EXT_JACK,
	NUM_MATRICE_PROTECT_1,
	NUM_MATRICE_PROTECT_2,
	NUM_PERSO,
	TYPE_INVENTAIRE,
	PROLOGUE,
	POIGNARD,
	MATRICE_FORME,
	MATRICE_COULEUR,
	
	UNKNOWN_CVAR = -1 // for table padding, shouldn't be called !
};

extern enumCVars AITD1KnownCVars[];
extern enumCVars AITD2KnownCVars[];

extern enumCVars* currentCVarTable;

int getCVarsIdx(enumCVars);
			
} // end of namespace Fitd

#endif
