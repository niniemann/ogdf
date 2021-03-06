#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

using namespace ogdf;

int main()
{
	Graph G;
	GraphAttributes GA(G,
	  GraphAttributes::nodeGraphics | GraphAttributes::nodeType |
	  GraphAttributes::edgeGraphics | GraphAttributes::edgeType);

	if (!GraphIO::read(GA, G, "ERDiagram.gml", GraphIO::readGML)) {
		cerr << "Could not read ERDiagram.gml" << endl;
		return 1;
	}

	PlanarizationLayout pl;

	SubgraphPlanarizer crossMin;

	auto* ps = new PlanarSubgraphFast<int>;
	ps->runs(100);
	VariableEmbeddingInserter *ves = new VariableEmbeddingInserter;
	ves->removeReinsert(RemoveReinsertType::All);

	crossMin.setSubgraph(ps);
	crossMin.setInserter(ves);

	EmbedderMinDepthMaxFaceLayers *emb = new EmbedderMinDepthMaxFaceLayers;
	pl.setEmbedder(emb);

	OrthoLayout *ol = new OrthoLayout;
	ol->separation(20.0);
	ol->cOverhang(0.4);
	pl.setPlanarLayouter(ol);

	pl.call(GA);

	GraphIO::write(GA, "output-ERDiagram.gml", GraphIO::writeGML);

	return 0;
}
