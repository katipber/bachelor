package cfs.data.entity;

import java.util.List;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToMany;

/**
 * Role entity class
 */

@Entity
public class Role {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long    id;
  private String name;
  
  @ManyToMany(mappedBy = "role")
  private List<Employee> employee;
  
  @Override
  public int hashCode() {
    return id.hashCode();
  }
  
  @Override
  public boolean equals(Object obj) {
    if(obj == null) return false;
    if(getClass() == obj.getClass())
      return hashCode() == obj.hashCode();
    return false;
  }

  public Role() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public List<Employee> getEmployee() {
    return employee;
  }

  public void setEmployee(List<Employee> employee) {
    this.employee = employee;
  }

}
