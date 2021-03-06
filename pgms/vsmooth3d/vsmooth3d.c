/****************************************************************
 *
 * Copyright (C) Max Planck Institute 
 * for Human Cognitive and Brain Sciences, Leipzig
 *
 * Author Gabriele Lohmann, 2004, <lipsia@cbs.mpg.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id: vsmooth3d.c 3177 2008-04-01 14:47:24Z karstenm $
 *
 *****************************************************************/

/*! \brief vsmooth3d - perform 3d smoothing filter

\par Description
vsmooth3d is a ranking filter used for smoothing segmentation results.
The filter computes the most frequent value in a 6 (or 18)
neighbourhood and replaces the center pixel with this value.
The pixels within the neighbourhood are weighted.
This program is useful for postprocessing of segmentation results.
It will produce smooth boundaries between regions.


\par Usage

        <code>vsmooth3d</code>

        \param -in      input image
        \param -out     output image
        \param -n       type of neighbourhood used for smoothing (6 | 18 | 26). Default: 18
        \param -iter    number of iterations. Default: 1


\par Examples
<br>
\par Known bugs
none.

\file vsmooth3d.c
\author G.Lohmann, MPI-CBS, 2004
*/


/* From the Vista library: */
#include <viaio/Vlib.h>
#include <viaio/mu.h>
#include <viaio/option.h>
#include <via.h>

/* From the standard C library: */
#include <stdio.h>
#include <stdlib.h>



VDictEntry TYPDict[] = {
  { "6", 0 },
  { "18", 1 },
  { "26", 2 },
  { NULL }
};

int main (int argc, char **argv)
{
  static VLong neighb = 1;
  static VLong iter = 1;
  static VOptionDescRec options[] = {
    { "n", VLongRepn, 1, &neighb, VOptionalOpt, TYPDict,
      "type of neighbourhood used for smoothing" },
    { "iter", VLongRepn, 1, &iter, VOptionalOpt, NULL,
      "number of iterations" }
  };

  FILE *in_file, *out_file;
  VAttrList list;
  VImage src, dest=NULL;
  VAttrListPosn posn;
  char prg[50];	
  sprintf(prg,"vsmooth3d V%s", getVersion());
  fprintf (stderr, "%s\n", prg);

  /* Parse command line arguments: */
  VParseFilterCmd (VNumber (options), options, argc, argv, &in_file, &out_file);
  
  /* Read source image(s): */
  if (! (list = VReadFile (in_file, NULL))) exit (1);

  /* process */
  for (VFirstAttr (list, & posn); VAttrExists (& posn); VNextAttr (& posn)) {
    if (VGetAttrRepn (& posn) != VImageRepn) continue;
    VGetAttrValue (& posn, NULL, VImageRepn, & src);

    dest = VSmoothImage3d (src, NULL,neighb,iter);
    if (dest == NULL) exit (1);
    VSetAttrValue (& posn, NULL, VImageRepn, dest);
    VDestroyImage (src);
  }

  /* Write the results to the output file: */
  VHistory(VNumber(options),options,prg,&list,&list);
  if (VWriteFile (out_file, list))
    fprintf (stderr, "%s: done.\n",argv[0]);
  return 0;
}
