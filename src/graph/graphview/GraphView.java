package graphview;

import java.awt.Dimension;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.LinkedList;

import javax.swing.JFrame;

import org.graphstream.algorithm.BetweennessCentrality;
import org.graphstream.algorithm.BetweennessCentrality.Progress;
import org.graphstream.graph.Edge;
import org.graphstream.graph.ElementNotFoundException;
import org.graphstream.graph.Graph;
import org.graphstream.graph.Node;
import org.graphstream.graph.implementations.MultiGraph;
import org.graphstream.stream.GraphParseException;
import org.graphstream.ui.swingViewer.View;
import org.graphstream.ui.swingViewer.Viewer;

public class GraphView {
	Graph g;
	Viewer viewer;
	View view;
	JFrame jf = null;
	
	private class Pair<K,V>  {
		K first;
		V second;
		public Pair(K f, V s) {
			first = f;
			second = s;
		}
		public K getFirst() { return first; }
		public V getSecond() { return second; }
		public boolean equals(Pair<K,V> p) {
			return (first==p.getFirst() && second==p.getSecond());
		}
		
	}
	
	HashMap<Integer, Node> nodeMap;
	Map<Pair<Integer,Integer>,Edge> edgeMap;
	
	public int  addNode(int id) {
		if (this.nodeMap.containsKey(id)) {
			return -1; // The node already exists
		} else {
			Node n = g.addNode(Integer.toString(id));
			n.setAttribute("ui.label", id);
			nodeMap.put(id, n);
			return n.getIndex();
		}
	}

	public void addEdge(int from, int to) {
		Node source = getNode(from);
		Node destin = getNode(to);
		if (source==null || destin==null) {
			System.err.println("Error");
			return;
		}
		String sfrom = Integer.toString(from);
		String sto   = Integer.toString(to);
		String edge = "("+sfrom+","+sto+")";
		Edge e = g.addEdge(edge,source.getIndex(),destin.getIndex());
		edgeMap.put(new Pair<Integer,Integer>(from,to), e);
		edgeMap.put(new Pair<Integer,Integer>(to,from), e);
	}
	
	private Node getNode(int id) {
		if (nodeMap.containsKey(id)){
			return nodeMap.get(id);
		} else
			return null;
	}
	
	private Edge getEdge(int from,int to) {
		Node source = getNode(from);
		Node destin = getNode(to);
		if (source==null || destin==null) {
			return null;
		}
		Pair<Integer,Integer> pii = new Pair<Integer,Integer>(from, to);
		for (Pair<Integer, Integer> x:edgeMap.keySet())  {
			if (x.equals(pii)) {
				return edgeMap.get(x);
			}
		}
		System.err.println("Error");
		return null;
	}
	

	
	
	private static void stop(long time) {
		try {
			Thread.sleep(time);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public void sendMessage_with_stop(int src, int dst,long time,String msg) {
		getNode(src).setAttribute("ui.class", "sent");
		getNode(dst).setAttribute("ui.class", "recv");
		Edge i = getEdge(src, dst);
		i.setAttribute("ui.class", "sent");
		i.setAttribute("ui.label", msg);
		stop(time);
		i.removeAttribute("ui.class");
		i.removeAttribute("ui.label");
		getNode(src).removeAttribute("ui.class");
		getNode(dst).removeAttribute("ui.class");
	}
	
	public void sendBroadcast_with_stop(int src, int[] dst,long time,String msg) {
		getNode(src).setAttribute("ui.class", "sent");
		LinkedList<Edge> ls = new LinkedList<Edge>();
		for (int x: dst) {
			getNode(x).setAttribute("ui.class", "recv");
			Edge i = getEdge(src, x);
			i.setAttribute("ui.class", "sent");
			i.setAttribute("ui.label", msg);
			ls.add(i);
		}
		stop(time);
		getNode(src).removeAttribute("ui.class");
		for (Edge i:ls) {
			i.removeAttribute("ui.class");
			i.removeAttribute("ui.label");
		}
		for (int x: dst) {
			getNode(x).removeAttribute("ui.class");
		}
	}
	
	public void betweenness() {
		BetweennessCentrality bcb = new BetweennessCentrality();
		bcb.setUnweighted();
		bcb.registerProgressIndicator(new Progress() {
			
			public void progress(float percent) {
				if (percent > 0.99) System.out.println(percent);
			}
		});
		bcb.init(g);
		bcb.compute();
		
	        double mine = Double.MAX_VALUE;
	        double maxe = Double.MIN_VALUE;
	 
	        // Obtain the maximum and minimum passes values.
	        for(Node edge: g.getEachNode()) {
	            double passes = edge.getAttribute("Cb");
	            if(passes>maxe) maxe = passes;
	            if(passes<mine) mine = passes;
	        }
	 
	        // Set the colors.
	        for(Node edge: g.getEachNode()) {
	            double passes = edge.getAttribute("Cb");
	            double color  = ((passes-mine)/(maxe-mine));
	            System.out.println(color);
	            edge.setAttribute("ui.color", color);
	        }
	    
		System.out.println(g.getNode(0).getAttribute("ui.color"));
	}
	
	public void degree() {
		double mine = Double.MAX_VALUE;
		double maxe = Double.MIN_VALUE;
		String css =  " graph {fill-mode: dyn-plain; } node { "
					+ "size:10px; text-background-mode: rounded-box;  text-alignment: at-right;"
					+ "shape: box; fill-mode: dyn-plain; fill-color: blue, red;"
					+"} edge {shape: freeplane;  size: 1px; fill-color: #444; } ";


		g.addAttribute("ui.stylesheet", css);
	 
		// Obtain the maximum and minimum passes values.
		for(Node edge: g.getEachNode()) {
		    double passes = edge.getDegree();
		    if(passes>maxe) maxe = passes;
		    if(passes<mine) mine = passes;
		}
	 
		// Set the colors.
		for(Node edge: g.getEachNode()) {
		    double passes = edge.getDegree();
		    double color  = ((passes-mine)/(maxe-mine));
		    System.out.println(color);
		    edge.setAttribute("ui.color", color);
		}
	}
	
	public GraphView() {
		System.setProperty("gs.ui.renderer", "org.graphstream.ui.j2dviewer.J2DGraphRenderer");
		g = new MultiGraph("grafo");
		nodeMap = new HashMap<Integer, Node>();
		edgeMap = new HashMap<Pair<Integer,Integer>,Edge>();
		//Stile di visualizzazione del grafo
		String css =  " graph {fill-mode: dyn-plain; } node { "
						+ "size:10px; text-background-mode: rounded-box;  text-alignment: at-right;"
						+ "shape: box; fill-mode: dyn-plain; fill-color: green, red;"
						+"} edge {  size: 1px; fill-color: #444; } edge.sent { fill-color: blue; } node.sent { fill-color:blue,blue; }";


		g.addAttribute("ui.stylesheet", css);
		g.addAttribute("ui.quality");
		g.addAttribute("ui.antialias");
		


		
	}

	
	public void show(String title) { 
		if (jf == null) {
			Viewer viewer = new Viewer(g, Viewer.ThreadingModel.GRAPH_IN_ANOTHER_THREAD);
			viewer.enableAutoLayout();
			View view = viewer.addDefaultView(false);
			jf = new JFrame();
			jf.setPreferredSize(new Dimension(800, 600));
		    	jf.pack();
		    	jf.setLocationRelativeTo(null);
			jf.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			jf.setResizable(true);
			jf.add(view);
		}
		jf.setTitle(title);
		jf.setVisible(true);
		
	}
	
	public void hide() {
		jf.setVisible(false);
	}
	
	public void close() {
		//XXX:jf.dispatchEvent(new WindowEvent(jf, WindowEvent.WINDOW_CLOSING));
		jf.dispose();
		jf = null;
	}
	
}

