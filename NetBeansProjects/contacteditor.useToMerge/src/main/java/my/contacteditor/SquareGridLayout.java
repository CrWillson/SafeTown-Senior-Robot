/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package my.contacteditor;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager;

/**
 *
 * @author sethcampbell150
 */
    public class SquareGridLayout implements LayoutManager {
    private int rows;
    private int cols;
    private int hgap;
    private int vgap;

    public SquareGridLayout(int rows, int cols, int hgap, int vgap) {
        this.rows = rows;
        this.cols = cols;
        this.hgap = hgap;
        this.vgap = vgap;
    }

    @Override
    public void addLayoutComponent(String name, Component comp) {}

    @Override
    public void removeLayoutComponent(Component comp) {}

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        Insets insets = parent.getInsets();
        int width = cols * (parent.getComponent(0).getPreferredSize().width + hgap) - hgap + insets.left + insets.right;
        int height = rows * (parent.getComponent(0).getPreferredSize().height + vgap) - vgap + insets.top + insets.bottom;
        return new Dimension(width, height);
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return preferredLayoutSize(parent);
    }

    @Override
    public void layoutContainer(Container parent) {
        Insets insets = parent.getInsets();
        int x = insets.left;
        int y = insets.top;
        int buttonWidth = parent.getComponent(0).getPreferredSize().width;
        int buttonHeight = parent.getComponent(0).getPreferredSize().height;

        for (Component comp : parent.getComponents()) {
            comp.setBounds(x, y, buttonWidth, buttonHeight);
            x += buttonWidth + hgap;
            if (x >= parent.getWidth() - insets.right) {
                x = insets.left;
                y += buttonHeight + vgap;
            }
        }
    }
}

